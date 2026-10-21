/*
 * A283901  Relative of Hofstadter Q-sequence: a(-310) = 4, a(-309) = 311,
 *          a(-308) = 4, a(-307) = 311; thereafter a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)).
 * https://oeis.org/A283901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) with a(1)=315, a(2)=4,
 *            a(3)=311, a(4)=315, a(n)=0 for n<=-311, and for -310<=k<=0 a(k)
 *            alternates 4,311 (a(-310)=4, a(-309)=311, …, a(0)=4)
 * Range:     u8 none, u16 n <= 658, u32 n <= 658, u64 n <= 658, u128 n <= 658,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static const idx_t MAX_INDEX = 658;          /* last defined term (a(658)=0) */

static seq_t *cache = NULL;                  /* a(0)…a(MAX_INDEX) */
static uint8_t *done  = NULL;                /* flag whether cache[i] is filled */

/* Helper: return a(k) for any signed index k (may be negative). */
static inline seq_t get_term_signed(int64_t k)
{
    if (k <= 0) {                            /* negative or zero indices */
        if (k <= -311) return (seq_t)0;
        /* For -310…0: alternate 4 and 311 starting with 4 at k=-310. */
        return (((((int64_t)k + 310) & 1) == 0) ? (seq_t)4 : (seq_t)311);
    }
    if (k > (int64_t)MAX_INDEX) return (seq_t)0;
    return cache[(size_t)k];
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* a(0) is unused, but defined as 0 to keep API simple. */
    if (n == 0) return (seq_t)0;
    if (n > MAX_INDEX) return (seq_t)0;

    /* Allocate cache on first use; use malloc in C, new[] in C++. */
    if (cache == NULL) {
#if defined(__cplusplus)
        cache = new seq_t[MAX_INDEX + 1];
        done  = new uint8_t[MAX_INDEX + 1];
#else
        cache = (seq_t *)calloc((size_t)(MAX_INDEX + 1), sizeof(seq_t));
        done  = (uint8_t *)calloc((size_t)(MAX_INDEX + 1), sizeof(uint8_t));
#endif
        if (!cache || !done) return (seq_t)0; /* allocation failed */
    }

    for (idx_t i = 1; i <= n; ++i) {
        if (done[i]) continue;               /* already computed */

        if (i == 1) {
            cache[i] = (seq_t)315;
        } else if (i == 2) {
            cache[i] = (seq_t)4;
        } else if (i == 3) {
            cache[i] = (seq_t)311;
        } else if (i == 4) {
            cache[i] = (seq_t)315;
        } else {
            int64_t idx1 = (int64_t)i - (int64_t)cache[i - 1];
            int64_t idx2 = (int64_t)i - (int64_t)cache[i - 2];
            cache[i] = get_term_signed(idx1) + get_term_signed(idx2);
        }
        done[i] = 1;
    }
    return cache[n];
}
