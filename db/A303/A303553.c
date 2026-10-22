/*
 * A303553  Number of periodic factorizations of n > 1 into positive factors
 *          greater than 1; a(1) = 1 by convention.
 * https://oeis.org/A303553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of periodic factorizations of n > 1 into positive
 *            factors > 1; a(1) = 1 by convention
 * Range:     u8 n <= 8929, u16 n <= 8929, u32 n <= 8929, u64 n <= 8929,
 *            u128 n <= 8929, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Helper: greatest common divisor for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: floor integer sqrt of idx_t using double sqrt for safety with small values */
static inline idx_t integer_sqrt(idx_t n) {
    if (n == (idx_t)-1) return (idx_t)-1; /* avoid overflow on sentinel */
    idx_t r = (idx_t) sqrt((double) n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* Process a completed factorization stored in fac[0..cnt-1] and increment answer if periodic. */
static void process_fact(idx_t fac[], int cnt, seq_t *answer) {
    idx_t cur_gcd = 0;
    int i = 0;
    while (i < cnt) {
        idx_t val   = fac[i];
        idx_t run   = 1;
        i++;
        while (i < cnt && fac[i] == val) {
            run++;
            i++;
        }
        cur_gcd = gcd_idx(cur_gcd, run);
    }
    if (cur_gcd > 1) {
        (*answer)++;
    }
}

/* Recursive enumeration of factorizations of rem starting with factors >= start. */
static void dfs(idx_t rem, idx_t start, idx_t fac[], int depth, seq_t *answer) {
    idx_t limit = integer_sqrt(rem);
    for (idx_t f = start; f <= limit; ++f) {
        if (rem % f == 0) {
            fac[depth] = f;
            dfs(rem / f, f, fac, depth + 1, answer);
        }
    }
    /* final factor (rem itself) */
    if (rem >= start && rem > 1) {
        fac[depth] = rem;
        process_fact(fac, depth + 1, answer);
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    seq_t count = 0;
    idx_t fac[64]; /* depth never exceeds log2(n) for n <= 10^4, 64 is safe */
    dfs(n, (idx_t)2, fac, 0, &count);
    return count;
}
