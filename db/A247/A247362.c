/*
 * A247362  Interleave A000861 and A247359, numbers ending in a vowel resp.
 *          consonant in English.
 * https://oeis.org/A247362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) is obtained by interleaving the numbers whose English *
 *            name ends in a vowel (even positions) with those that end in a
 *            consonant * (odd positions)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Local helper: determines the last letter of the English name of n.
 * Works for all non‑negative numbers. */
static inline char last_letter(idx_t n)
{
    static const char unit_last[10] = {
        'o','e','o','e','r','e','x','n','t','e'   /* 0 – 9 */
    };
    static const char teen_last[10] = {
        'n','n','e','n','n','n','n','n','n','n'   /* 10 – 19 */
    };

    if (n < 10) return unit_last[(int)n];
    if (n < 20) return teen_last[(int)(n-10)];

    if (n < 100) {
        unsigned int unit = (unsigned int)(n % 10);
        if (unit == 0) return 'y';          /* "twenty", "thirty", … */
        return unit_last[unit];
    }

    if (n < 1000) {
        unsigned int rem = (unsigned int)(n % 100);
        if (rem == 0) return 'd';          /* "hundred" */
        return last_letter(rem);
    }

    /* For larger numbers, express in thousands, millions, etc. */
    idx_t pow = 1;
    while (pow <= n / 1000) pow *= 1000;    /* largest power of 1000 ≤ n */
    idx_t rem = n % pow;

    if (rem == 0) {                         /* exact power of 1000 */
        return (pow == 1000) ? 'd' : 'n';   /* thousand → 'd', others → 'n' */
    }
    return last_letter(rem);
}

static inline bool ends_in_vowel(idx_t n)
{
    char l = last_letter(n);
    return l=='a' || l=='e' || l=='i' || l=='o' || l=='u';
}

seq_t a(idx_t n)
{
    /* How many vowel and consonant elements are needed up to index n? */
    idx_t need_v = n/2 + 1;          /* even positions: 0,2,4,… */
    idx_t need_c = (n+1)/2;          /* odd positions: 1,3,5,… */

#ifdef __cplusplus
    /* C++ implementation using std::vector for automatic construction */
    static std::vector<seq_t> vowels;
    static std::vector<seq_t> consonants;
    static idx_t next = 0;           /* next integer to examine */

    while (vowels.size() < need_v || consonants.size() < need_c) {
        if (ends_in_vowel(next)) {
            vowels.emplace_back((seq_t)next);
        } else {
            consonants.emplace_back((seq_t)next);
        }
        ++next;
    }

    return (n & 1) ? consonants[(n-1)/2] : vowels[n/2];
#else
    /* C implementation using manual realloc (works for trivially
     * copyable seq_t types such as uint8_t … uint64_t). */
    static seq_t *vowels  = NULL, *consonants = NULL;
    static size_t  vcap   = 0,     ccap   = 0;
    static idx_t   vcnt   = 0,     ccnt   = 0;
    static idx_t   next   = 0;

    while (vcnt < need_v || ccnt < need_c) {
        bool v = ends_in_vowel(next);
        if (v) {
            if (vcnt == vcap) {
                size_t newcap = vcap == 0 ? 4 : vcap * 2;
                seq_t *tmp = (seq_t *)realloc(vowels, newcap * sizeof(seq_t));
                vowels = tmp; vcap = newcap;
            }
            vowels[vcnt++] = (seq_t)next;
        } else {
            if (ccnt == ccap) {
                size_t newcap = ccap == 0 ? 4 : ccap * 2;
                seq_t *tmp = (seq_t *)realloc(consonants, newcap * sizeof(seq_t));
                consonants = tmp; ccap = newcap;
            }
            consonants[ccnt++] = (seq_t)next;
        }
        ++next;
    }

    return (n & 1) ? consonants[(n-1)/2] : vowels[n/2];
#endif
}
