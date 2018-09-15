# -*- coding: utf-8 -*-
import scrapy
from bs4 import BeautifulSoup
from scrapy.spiders import CrawlSpider, Rule
from scrapy.linkextractors import LinkExtractor
from wiki.items import WikiItem

class WikipedialSpider(CrawlSpider):
    name = "wikipedia_spider"
    
    allowed_domains = ["en.wikipedia.org"]
    
    start_urls = ['https://en.wikipedia.org/wiki/Kotlin_(programming_language)',
                  'https://en.wikipedia.org/wiki/Jimi_Hendrix',
                  'https://en.wikipedia.org/wiki/In_Bruges',
                  'https://en.wikipedia.org/wiki/Jägermeister',
                  'https://en.wikipedia.org/wiki/Orange_(fruit)']

    link_extractor = LinkExtractor(
        allow='https://en\.wikipedia\.org/wiki/.*',
        deny='https://en\.wikipedia\.org/wiki/' + \
             '(' + \
             '(File|Talk|Category|Portal|Special|Template|Template_talk|Wikipedia|Help|Draft|):.*|' + \
             'Main_Page' + \
             ')',
        restrict_xpaths=['//div[@id="mw-content-text"]/*/*/a']
    )

    rules = (
        Rule(
            link_extractor=link_extractor, 
            callback='parse_wiki_page', 
            follow=True,
            process_links='limit_links'
        ),
    )

    def limit_links(self, links):
        return links[:100]

    def parse_wiki_page(self, response):
        #print("Parsed: ", response.url)
        
        item = WikiItem()
        
        item['url'] = response.url

        item['title'] = BeautifulSoup(
            response.xpath('//h1[@id="firstHeading"]').extract_first(), "lxml"
        ).text

        item['info'] = BeautifulSoup(
            response.xpath('//div[@id="mw-content-text"]/*/p[1]').extract_first(), "lxml"
        ).text[:255] + "..."

        #print("Links: ", list(map(lambda link: link.url,
        #                   self.link_extractor.extract_links(response))))

        page_urls = set(map(lambda link: link.url,
                            self.link_extractor.extract_links(response)))
        page_urls.discard(response.url)
        item['out_urls'] = page_urls

        return item