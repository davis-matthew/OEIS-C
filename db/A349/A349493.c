/*
 * A349493  a(1)=1, a(2)=2; for n > 2, a(n) is the smallest unused positive
 *          number such that gcd(a(n-2)+a(n-1), a(n)) > 1 while gcd(a(n-2),
 *          a(n)) = 1 and gcd(a(n-1), a(n)) = 1.
 * https://oeis.org/A349493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A349493
 * a(1)=1, a(2)=2; for n>2, a(n) is the smallest unused positive integer
 * such that
 *     gcd(a(n-2)+a(n-1), a(n)) > 1   &&   gcd(a(n-2), a(n)) = 1
 *     && gcd(a(n-1), a(n)) = 1.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static void ensure_used_cap(size_t needed, uint8_t **used, size_t *used_cap)
{
    if (needed < *used_cap) return;
    size_t new_cap = (*used_cap == 0) ? 4 : *used_cap << 1;
    while (new_cap <= needed) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(*used, new_cap);
    if (!tmp) abort();
    memset(tmp + *used_cap, 0, new_cap - *used_cap);
    *used = tmp;
    *used_cap = new_cap;
}

static void ensure_seq_cap(size_t needed, seq_t **seq, size_t *seq_cap)
{
    if (needed < *seq_cap) return;
    size_t new_cap = (*seq_cap == 0) ? 4 : *seq_cap << 1;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *seq = tmp;
    *seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;

    static uint8_t *used   = NULL;
    static size_t used_cap = 0;

    static idx_t smallest_unused = 1;

    if (n == 0) return 0;   /* offset starts at 1 */

    /* Ensure storage for the requested index */
    ensure_seq_cap((size_t)n, &seq, &seq_cap);
    if (seq_len < (size_t)n) {
        /* generate missing terms */
        for (size_t i = seq_len; i < (size_t)n; ++i) {
            idx_t idx = (idx_t)(i + 1);  /* 1‑based index */
            seq_t value;

            if (idx == 1)
                value = (seq_t)1;
            else if (idx == 2)
                value = (seq_t)2;
            else {
                seq_t prev2 = seq[i - 2];
                seq_t prev1 = seq[i - 1];
                seq_t sum   = prev2 + prev1;

                seq_t cand = (seq_t)smallest_unused;
                for (;;) {
                    /* make sure used[] can hold cand */
                    ensure_used_cap((size_t)cand, &used, &used_cap);

                    if (used[(size_t)cand]) {
                        cand++;
                        continue;
                    }
                    if (gcd(prev2, cand) != 1 ||
                        gcd(prev1, cand) != 1 ||
                        gcd(sum,   cand) <= 1) {
                        cand++;
                        continue;
                    }
                    break;
                }
                value = cand;
            }

            seq[i] = value;

            /* mark the value as used */
            ensure_used_cap((size_t)value, &used, &used_cap);
            used[(size_t)value] = 1;

            /* update smallest_unused */
            while ((size_t)smallest_unused < used_cap &&
                   used[(size_t)smallest_unused])
                smallest_unused++;
        }
        seq_len = (size_t)n;
    }

    return seq[n - 1];
}
