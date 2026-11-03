/* digit_counts: how often each decimal digit occurs in n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_COUNTS_H
#define OEISC_UTILS_DIGIT_COUNTS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---------- digit counting ----------------------------------------------- */
static void digit_counts(seq_t x, unsigned char cnt[10])
{
    for (int d = 0; d < 10; ++d) cnt[d] = 0;
    if (x == 0) { cnt[0] = 1; return; }
    while (x > (seq_t)0) {
        unsigned digit = (unsigned)(x % (seq_t)10);
        ++cnt[digit];
        x /= (seq_t)10;
    }
}

#endif
