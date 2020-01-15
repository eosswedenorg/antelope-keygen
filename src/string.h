/**
 * MIT License
 *
 * Copyright (c) 2019-2020 EOS Sw/eden
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef STRING_H
#define STRING_H

#include <vector>
#include <string>

typedef std::vector<std::string> strlist_t;

strlist_t strsplitwords(const std::string& str, const std::string& delim = ",");

strlist_t strsplit(const std::string& str, const std::string& delim);

std::string strjoin(const strlist_t& list, const std::string& delim);

std::string& strtolower(std::string& str);

std::string& rtrim(std::string& str);
std::string& ltrim(std::string& str);
std::string& trim(std::string& str);

std::string& base58_strip(std::string &str);
strlist_t& base58_strip(strlist_t& list);

strlist_t l33twords(std::string str);

#endif /* STRING_H */
