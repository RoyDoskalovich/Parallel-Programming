#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>  // Include SSE intrinsics header

// Remove all characters except a-z, A-Z, 0-9, +, /
void remove_characters(char * str){
    int length = strlen(str);
    int count=0;
    char * newStr = (char *)malloc((length + 1) * sizeof(char));    // Allocate memory for the new string
    for(int i=0;i<length;i++){  // Iterate through the string
        char c = str[i];
        if(c >= 'a' && c <= 'z'){       // Check if the character is a lowercase letter
            newStr[count] = c;
            count++;
            continue;
        }
        if(c >= 'A' && c <= 'Z'){    // Check if the character is an uppercase letter
            newStr[count] = c;
            count++;
            continue;
        }
        if(c == '+'){   // Check if the character is a '+'
            newStr[count] = c;
            count++;
            continue;
        }
        if(c == '/'){   // Check if the character is a '/'
            newStr[count] = c;
            count++;
            continue;
        }
        if(c >= '0' && c <= '9'){   // Check if the character is a digit
            newStr[count] = c;
            count++;
            continue;
        }
    }
    newStr[count] = '\0';   // Null-terminate the new string
    strcpy(str,newStr); // Copy the new string to the original string
    free(newStr);   // Free the memory allocated for the new string
}

// Encode a Base64 string using SSE2 instructions
int b64_encode(const char* str) {
    int length = strlen(str);
    int value = 0;

    // Create a mask to extract the lower 6 bits of each character
    __m128i shift = _mm_setr_epi8(26, 0, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 0);

    int i;
    for (i = 0; i <= length - 16; i += 16) {    // Process 16 characters at a time
        __m128i input = _mm_loadu_si128((__m128i*)(str + i));   // Load 16 characters
        __m128i shuf = _mm_shuffle_epi32(input, _MM_SHUFFLE(0, 3, 2, 1));   // Shuffle the characters
        shuf = _mm_and_si128(shuf, _mm_set1_epi32(0x3F3F3F3F));  // Mask to extract the lower 6 bits
        __m128i shifted = _mm_srl_epi64(shuf, _mm_set_epi64x(0, 12));   // Shift the bits to form indices for b64chars
        __m128i scaled = _mm_madd_epi16(shifted, shift);    // Scale the indices
        value += _mm_cvtsi128_si32(scaled);   // Store the result
    }

    // Handle the remaining characters (less than 16)
    for (; i < length; i++) {
        char c = str[i];
        if (c >= 'a' && c <= 'z') {
            value = value * 64 + (c - 'a' + 26);
        } else if (c >= 'A' && c <= 'Z') {
            value = value * 64 + (c - 'A');
        } else if (c == '+') {
            value = value * 64 + 62;
        } else if (c == '/') {
            value = value * 64 + 63;
        } else if (c >= '0' && c <= '9') {
            value = value * 64 + (c - '0' + 52);
        }
    }

    return value;
}

int b64_distance(char * str1, char * str2){
    remove_characters(str1);    // Remove all characters except a-z, A-Z, 0-9, +, /
    remove_characters(str2);    // Remove all characters except a-z, A-Z, 0-9, +, /
    int result1 = b64_encode(str1); // Encode the first string
    int result2 = b64_encode(str2); // Encode the second string
    return result2 - result1;   // Return the difference between the two encoded strings
}

