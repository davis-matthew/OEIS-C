/*
 * A371478  Numbers k for which k swaps are needed to bubble-sort the US
 *          English number name of k.
 * https://oeis.org/A371478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   swap_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth element of
   A371478 – the numbers k such that k swaps are needed to
   bubble‑sort the US English name of k.  The algorithm
   scans all integers up to 2000, counts inversions in their
   letter names, and stores those where the count equals the
   integer itself.  The sequence is therefore generated on
   first use and cached for subsequent calls. */

#include <stdbool.h>
#include <string.h>
#include "../utils/swap_count.h"

static const char *ones[] = {"", "one","two","three","four","five",
                             "six","seven","eight","nine"};
static const char *teens[] = {"ten","eleven","twelve","thirteen","fourteen",
                              "fifteen","sixteen","seventeen","eighteen","nineteen"};
static const char *tens[]  = {"","","twenty","thirty","forty","fifty",
                              "sixty","seventy","eighty","ninety"};

static void number_name(idx_t n, char *buf, size_t *len)
{
    if (n >= 1000000) {
        number_name(n / 1000000, buf, len);
        strcat(buf, "million");
        *len += 7;
        n %= 1000000;
    }
    if (n >= 1000) {
        number_name(n / 1000, buf, len);
        strcat(buf, "thousand");
        *len += 8;
        n %= 1000;
    }
    if (n >= 100) {
        number_name(n / 100, buf, len);
        strcat(buf, "hundred");
        *len += 7;
        n %= 100;
    }
    if (n >= 20) {
        strcat(buf, tens[n / 10]);
        *len += strlen(tens[n / 10]);
        n %= 10;
    }
    if (n >= 10) {
        strcat(buf, teens[n - 10]);
        *len += strlen(teens[n - 10]);
        n = 0;
    }
    if (n > 0 && n < 10) {
        strcat(buf, ones[n]);
        *len += strlen(ones[n]);
    }
}

seq_t a(idx_t n)
{
    /* local cache of found terms */
    static seq_t terms[1000];
    static size_t terms_len = 0;
    static bool ready = false;

    if (!ready) {
        const idx_t MAX_K = 2000;          /* plenty above the theoretical 712 limit */
        char buf[256];
        for (idx_t k = 2; k <= MAX_K; ++k) {
            buf[0] = '\0';
            size_t len = 0;
            number_name(k, buf, &len);
            uint32_t sc = swap_count(buf, len);
            if (sc == k) {
                if (terms_len < sizeof(terms)/sizeof(terms[0]))
                    terms[terms_len++] = k;
            }
        }
        ready = true;
    }

    if (n == 0 || n > terms_len)
        return 0;           /* out of range */

    return terms[n - 1];
}
