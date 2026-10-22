/*
 * A340068  a(n) is the number of integers in the set {n+1, n+2, ..., 2*n}
 *          whose representation in base 2 contain exactly three digits 1's.
 * https://oeis.org/A340068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   binom1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A340068: a(n) = # { m | n < m ≤ 2n, popcount(m) = 3 }.
 *                          = count_pop3(2n) – count_pop3(n)
 *                          where count_pop3(x) counts integers in [0,x] with
 *                          exactly 3 ones in binary.
 */
#include <stdint.h>
#include "../utils/binom1.h"

static inline seq_t binom3(int i) {
    /* Binomial coefficient C(i,3) for i >= 3, otherwise 0. */
    if (i < 3) return 0;
    return (seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2) / 6;
}

static inline seq_t binom2(int i) {
    if (i < 2) return 0;
    return (seq_t)i * (seq_t)(i - 1) / 2;
}

static seq_t count_pop3(idx_t x) {
    /* Count numbers in [0, x] with popcount == 3. */
    seq_t total = 0;
    int need = 3;                 /* remaining ones to place */
    /* Find most significant bit of x. */
    int msb = -1;
    for (int i = 63; i >= 0; --i)
        if ((x >> i) & 1ULL) { msb = i; break; }

    if (msb == -1) return 0;      /* x == 0 */

    for (int i = msb; i >= 0; --i) {
        if ((x >> i) & 1ULL) {
            /* We may put 0 here and choose 'need' ones in lower i bits. */
            if (need == 3) total += binom3(i);
            else if (need == 2) total += binom2(i);
            else if (need == 1) total += binom1(i);
            else if (need == 0) break;  /* all ones used */

            /* Use this 1 bit. */
            --need;
            if (need == 0) break;      /* no more ones to place */
        }
    }
    return total;
}

seq_t a(idx_t n) {
    /* Compute a(n) for any n ≥ 1.  Handles n up to 64‑bit limits. */
    /* 2*n may overflow 64‑bit if n > 2^63-1; we guard with 128‑bit. */
    unsigned __int128 two_n = (unsigned __int128)n * 2;
    idx_t max = (idx_t)two_n;          /* fits into idx_t for our limits */
    seq_t fmax = count_pop3(max);
    seq_t fn   = count_pop3(n);
    return fmax - fn;
}
