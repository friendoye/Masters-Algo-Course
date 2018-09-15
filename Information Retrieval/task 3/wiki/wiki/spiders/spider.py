# -*- coding: utf-8 -*-
import scrapy
from bs4 import BeautifulSoup
from scrapy.spiders import CrawlSpider, Rule
from scrapy.linkextractors import LinkExtractor
from wiki.items import WikiItem

def get_wiki_pages_urls_with_index(file_name = 'urlid.csv'):
    wiki_dict = dict()
    with open(file_name) as urls_file:
        base_url = 'https://ru.wikipedia.org'   
        for line in urls_file:
            index, encoded_segment = line.strip().split(',', 1)
            url = base_url + encoded_segment
            wiki_dict[url] = index
    print(str(wiki_dict))
    return wiki_dict

class WikipedialSpider(CrawlSpider):
    name = "wikipedia_spider"
    
    wiki_url_dict = get_wiki_pages_urls_with_index()
    allowed_domains = ["ru.wikipedia.org"]
    
    start_urls = list(wiki_url_dict.keys())

    def parse(self, response):
        yield self.parse_wiki_page(response)

    def parse_wiki_page(self, response):
        #print("Parsed: ", response.url)
        
        item = WikiItem()
        
        item['url'] = response.url

        item['title'] = BeautifulSoup(
            response.xpath('//h1[@id="firstHeading"]').extract_first(), "lxml"
        ).text

        item['info'] = BeautifulSoup(
            response.xpath('//div[@id="mw-content-text"]/*/p[1]').extract_first(), "lxml"
        ).text

        item['index'] = self.wiki_url_dict[response.url]

        return item