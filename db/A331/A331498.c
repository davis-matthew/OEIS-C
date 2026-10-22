/*
 * A331498  Position of the first letter of n (in French) in the alphabet.
 * https://oeis.org/A331498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = position (1‑26) of the first letter of the French
 *            cardinal word for n in the alphabet
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   tens_letter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tens_letter.h"

static const char letter_map_0_19[20] = {
    'Z', /* 0 "zéro"   */
    'U', /* 1 "un"     */
    'D', /* 2 "deux"   */
    'T', /* 3 "trois"  */
    'Q', /* 4 "quatre" */
    'C', /* 5 "cinq"   */
    'S', /* 6 "six"    */
    'S', /* 7 "sept"   */
    'H', /* 8 "huit"   */
    'N', /* 9 "neuf"   */
    'D', /*10 "dix"    */
    'O', /*11 "onze"   */
    'D', /*12 "douze"  */
    'T', /*13 "treize" */
    'Q', /*14 "quatorze"*/
    'Q', /*15 "quinze" */
    'S', /*16 "seize"  */
    'D', /*17 "dix‑sept"*/
    'D', /*18 "dix‑huit"*/
    'D'  /*19 "dix‑neuf"*/
};

/* Return the uppercase letter that begins the French word for n */
static char first_letter(uint64_t n)
{
    if (n == 0) return 'Z';
    if (n < 20) return letter_map_0_19[n];

    if (n < 100) {
        uint64_t tens = n / 10;
        if (tens == 7) return 'S';        /* 70‑79 "soixante‑..." */
        if (tens == 8) return 'Q';        /* 80‑89 "quatre‑vingt" */
        if (tens == 9) return 'Q';        /* 90‑99 "quatre‑vingt‑" */
        return tens_letter(tens);         /* 20‑69 appropriate letter */
    }

    if (n < 1000) {
        uint64_t hundred = n / 100;
        if (hundred == 1) return 'C';     /* 100‑199 "cent..." */
        return letter_map_0_19[hundred]; /* 200‑999 use the digit’s letter */
    }

    /* n is 1000 or more */
    uint64_t thousand = n / 1000;
    if (thousand == 1) return 'M';        /* 1000 "mille" */
    return first_letter(thousand);        /* thousands part determines it */
}

seq_t a(idx_t n)
{
    /* Convert the first letter (uppercase) to its position 1‑26 */
    char c = first_letter((uint64_t)n);
    return (seq_t)(c - 'A' + 1);
}
