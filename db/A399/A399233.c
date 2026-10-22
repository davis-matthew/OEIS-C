/*
 * A399233  Number of minimal connected dominating sets in the n-antiprism
 *          graph.
 * https://oeis.org/A399233
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399233: Number of minimal connected dominating sets in the n‑antiprism graph.
 * Recurrence (valid for n ≥ 9):
 *   a(n) = 4*a(n-1) - 6*a(n-2) + 6*a(n-3) - 5*a(n-4) + 2*a(n-5) - a(n-6)
 * Initial values:
 *   a(3)=12, a(4)=24, a(5)=40, a(6)=84, a(7)=182, a(8)=368
 * Offset is 3: values for n<3 are undefined (return 0).
 */

#include <stddef.h>

/* Maximum n we will support (≥ 10000, chosen a bit higher for safety). */
#define MAX_N 20001

static seq_t cache[MAX_N];        /* values for 0 … MAX_N-1 */
static size_t cache_max = 2;      /* highest index already computed (initially up to 2) */

/* Ensure that cache[3…n] have been computed. */
static void ensure_cache(size_t n)
{
    if (n <= cache_max) return;          /* nothing to do */

    /* Base values for indices 3–8 */
    static const seq_t base[9] = {
        0,                  /* 0 */
        0,                  /* 1 */
        0,                  /* 2 */
        12,                 /* 3 */
        24,                 /* 4 */
        40,                 /* 5 */
        84,                 /* 6 */
        182,                /* 7 */
        368                 /* 8 */
    };

    for (size_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = base[i];
        } else {
            /* recurrence for n ≥ 9 */
            seq_t term = 4 * cache[i-1];
            term -= 6 * cache[i-2];
            term += 6 * cache[i-3];
            term -= 5 * cache[i-4];
            term += 2 * cache[i-5];
            term -= cache[i-6];
            cache[i] = term;
        }
    }
    cache_max = n;
}

/* Return the nth term of the sequence (for n ≥ 3). */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* undefined below offset */
    size_t idx = (size_t)n;
    /* Guard against requests beyond the pre‑allocated array */
    if (idx >= MAX_N) return 0;         /* out of bounds – value undefined */
    ensure_cache(idx);
    return cache[idx];
}
