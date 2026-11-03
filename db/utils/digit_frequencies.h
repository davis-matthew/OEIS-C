/* digit_frequencies: how often each decimal digit occurs in n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_FREQUENCIES_H
#define OEISC_UTILS_DIGIT_FREQUENCIES_H
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
static void digit_frequencies(seq_t x, unsigned int freq[10])
{
    for (int i = 0; i < 10; ++i) freq[i] = 0;
    if (x == 0) {               /* digit '0' appears once */
        freq[0] = 1;
        return;
    }
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        ++freq[d];
        x /= 10;
    }
}

#endif
