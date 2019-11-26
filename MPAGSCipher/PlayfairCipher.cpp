#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "CipherMode.hpp"
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher(const std::string& key) 
{
  this->setKey(key);
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
  key_.erase(non_alpha_edited,key_.end());

  // Change J -> I
  std::transform(std::begin(key_),std::end(key_),std::begin(key_),[](char chr){return (chr == 'J') ? 'I' : chr;});  

  // Remove duplicated letters
  std::string format_dup_ltr{""};
  auto detectDuplicates = [&format_dup_ltr](char chr) {
  if (format_dup_ltr.find(chr) == std::string::npos )
     {
       format_dup_ltr += chr;
       return false;
     }
  else 
     {
       return true;
     }
   };
  auto no_duplicates_edited = std::remove_if(std::begin(key_),std::end(key_),detectDuplicates);
  key_.erase(no_duplicates_edited,key_.end());

  // Store the coords of each letter
  for (size_t i=0; i < key_.size(); i++) 
   {
     const size_t column{i%5};
     const size_t row{i/5};
     const auto coords = std::make_pair(column,row);

     /* or, with the new typedef, can do
     const PlayfairCoords coords { i%5, i/5 };
     */

  // Store the playfair cipher key map
     char_to_coord[key_[i]] = coords;
     coord_to_char[coords] = key_[i];
   }
}

std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode ) const
{
  std::string outputText {inputText};

  // Change J-> I
  std::transform(std::begin(outputText),std::end(outputText),std::begin(outputText),[](char chr){return (chr == 'J') ? 'I' : chr;});

  // If repeated chars in a digraph add an X or Q if XX
  for (size_t i=0; i<outputText.size() ; i+=2 ) 
     {
       if (outputText[i] == outputText[i+1] && outputText[i] == 'X')
         {
           outputText.insert(i+1,"Q");
         }
       else if (outputText[i] == outputText[i+1])
         {
           outputText.insert(i+1,"X");
         }
      }
 
  // if the size of input is odd, add a trailing Z
  if ((outputText.size()%2) == 1)
    {
      outputText += (outputText[outputText.size()-1] == 'Z') ? 'X' : 'Z';
    }

  // Determine the shift to apply to row/column operations
  const size_t shift { (cipherMode == CipherMode::Encrypt) ? 1u : 4u };
  
  // Loop over the input in Digraphs
  for (size_t i=0; i< outputText.size(); i+=2)
    {
  //  - Find the coords in the grid for each digraph
      const auto coord_1 = char_to_coord.find(outputText[i])->second;
      const auto coord_2 = char_to_coord.find(outputText[i+1])->second;
      auto new_coord_1{coord_1};
      auto new_coord_2{coord_2};
  //  - Apply the rules to these coords to get 'new' coords
      if (coord_1.first == coord_2.first)
        {
          new_coord_1.second = (coord_1.second+shift)%5;   
          new_coord_2.second = (coord_2.second+shift)%5;
        }
      else if (coord_1.second == coord_2.second)
        {
          new_coord_1.first = (coord_1.first+shift)%5;
          new_coord_2.first = (coord_2.first+shift)%5;
        }
      else
        {
          std::swap( new_coord_1.first, new_coord_2.first );
        }
  //  - Find the letter associated with the new coords
        outputText[i]   = coord_to_char.at(new_coord_1);
        outputText[i+1] = coord_to_char.at(new_coord_2);
      }  

  //  return the text
  //
  return outputText;
}
 
