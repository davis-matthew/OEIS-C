/*
 * A332301  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest divisor of the
 *          sum of the previous two terms that has not yet appeared. If all
 *          divisors have appeared then a(n) equals the sum.
 * https://oeis.org/A332301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest divisor of
 *            a(n-1) + a(n-2) that has not appeared before. If all divisors
 *            have appeared, a(n) equals the sum
 * Range:     u8 n <= 54, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;          /* array of computed terms, 0‑based */
static size_t seq_len = 0;         /* number of terms stored */
static size_t seq_cap = 0;         /* capacity of seq */

static seq_t *seen = NULL;         /* list of seen values */
static size_t seen_len = 0;
static size_t seen_cap = 0;

/* make sure the array can hold at least 'need' elements */
static void ensure_seq_cap(size_t need) {
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

static void ensure_seen_cap(size_t need) {
    if (need <= seen_cap) return;
    size_t newcap = seen_cap ? seen_cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    seen = (seq_t *)realloc(seen, newcap * sizeof(seq_t));
    seen_cap = newcap;
}

/* linear search for membership, returns 1 if found */
static int seen_contains(seq_t v) {
    for (size_t i = 0; i < seen_len; ++i)
        if (seen[i] == v) return 1;
    return 0;
}

/* append a value to seq and seen arrays */
static void append(seq_t v) {
    ensure_seq_cap(seq_len + 1);
    seq[seq_len++] = v;
    ensure_seen_cap(seen_len + 1);
    seen[seen_len++] = v;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n<1, return 0 as placeholder */

    /* initialise on first call */
    if (seq_len == 0) {
        append((seq_t)1);          /* a(1) */
        append((seq_t)2);          /* a(2) */
    }

    /* compute terms up to n */
    while (seq_len < n) {
        seq_t prev1 = seq[seq_len - 1];
        seq_t prev2 = seq[seq_len - 2];
        seq_t sum   = prev1 + prev2;

        /* find smallest divisor of sum not yet seen */
        seq_t smallest = sum + (seq_t)1;   /* marker larger than any divisor */

        for (idx_t d = 1; (seq_t)d * (seq_t)d <= sum; ++d) {
            seq_t div = (seq_t)d;
            if (sum % div == 0) {
                if (!seen_contains(div) && div < smallest)
                    smallest = div;
                seq_t partner = sum / div;
                if (partner != div && !seen_contains(partner) && partner < smallest)
                    smallest = partner;
            }
        }

        seq_t next = (smallest == sum + (seq_t)1) ? sum : smallest;
        append(next);
    }

    return seq[n - 1];
}
