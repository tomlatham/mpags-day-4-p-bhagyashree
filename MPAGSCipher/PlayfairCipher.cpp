#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "CipherMode.hpp"
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher(const std::string& key) 
{
  key_ = key;
}

void PlayfairCipher::setKey(const std::string& key) 
{
  const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  // store the original key
  key_ = key;

  // Append the alphabet
  key_ +=alphabet;

  // Make sure the key is upper case
  std::transform(std::begin(key_), std::end(key_), std::begin(key_),::toupper);

  // Remove non-alpha characters
  auto non_alpha_edited = std::remove_if(std::begin(key_),std::end(key_),[](char chr) {return(!isalpha(chr));});
  key_.erase(non_alpha_edited);

  // Change J -> I
  std::transform(std::begin(key_),std::end(key_),std::begin(key_),[](char chr){return (chr == 'J') ? 'I' : chr;});  

  // Remove duplicated letters
  std::string format_dup_ltr{""};
  std::remove_if(std::begin(key_),std::end(key_),[&format_dup_ltr](char chr) {
  if (format_dup_ltr.find(chr) == std::string::npos )
     {
       format_dup_ltr += chr;
       return false;
     }
  else 
     {
       return true;
     }
   });//, std::end(key_));

  // Store the coords of each letter
  for (size_t i=0; i < key_.size(); i++) 
   {
     size_t X{i%5};
     size_t Y{i/5};
     auto coords = std::make_pair(X,Y);

  // Store the playfair cipher key map
    // char chr = key_[i];
     char_to_coord[key_[i]] = coords;       // query over here
     coord_to_char[coords] = key_[i];
   }
}

std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode ) const
{
  // Change J-> I
  std::transform(std::begin(inputText),std::end(inputText),std::begin(inputText),[](char chr){return (chr == 'J') ? 'I' : chr;});

  // If repeated chars in a digraph add an X or Q if XX
  for (size_t i=0; i<inputText.size() ; i++ ) 
     {
       if (inputText[i] == inputText[i+1] && inputText[i] == 'X')
         {
           //inputText[i] ='Q';
           inputText.insert(i,"Q");
         }
       else if (inputText[i] == inputText[i-1])
         {
           inputText[i] = 'X';
         }
      }
 
  // if the size of input is odd, add a trailing Z
  if ((inputText.size()%2) == 1)
    {
      inputText += 'Z';
    }
  
  // Loop over the input in Digraphs
  for (size_t i=0; i< 2*inputText.size(); i+=2)
    {
  //  - Find the coords in the grid for each digraph
      auto coord_1, coord_2, new_coord_1, new_coord_2;
      coord_1 = char_to_coord.find(inputText[i])->second;
      coord_2 = char_to_coord.find(inputText[i+1])->second;
  //  - Apply the rules to these coords to get 'new' coords
      if (coord_1.X == coord_2.X)
        {
          new_coord_1.Y = (coord_1.Y+1)%5;   
          new_coord_2.Y = (coord_2.Y+1)%5;
        }
      else (coord_1.Y == coord_2.Y)
        {
          new_coord_1.X = (coord_1.X+1)%5;
          new_coord_2.X = (coord_2.X+1)%5;
        }
      else
        {
          new_coord_1.Y = (coord_1.Y+4)%5;
          new_coord_2.Y = (coord_2.Y+4)%5;
        }
  //  - Find the letter associated with the new coords
        auto input = coord_to_char.find(new_coord_1, new_coord_2);
      }  

  //  return the text
  //
  return input;
}
 
