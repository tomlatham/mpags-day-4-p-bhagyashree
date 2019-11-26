#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include <string>
#include <vector>
#include <map>

#include "CipherMode.hpp"

class PlayfairCipher {
  public:

    explicit PlayfairCipher(const std::string& key);
    void setKey(const std::string& key);
    std::string applyCipher( const std::string& inputText, const CipherMode cipherMode ) const;

  private:    
    std::string key_ = "";
    std::map<char, std::pair<size_t, size_t>> char_to_coord;
    std::map<std::pair<size_t,size_t>, char> coord_to_char;

};

#endif
