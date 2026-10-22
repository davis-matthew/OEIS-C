/*
 * A370265  Rectangular array, read by antidiagonals: T(n,k) = greatest m such
 *          that 2^m divides prime(n+k+1)-prime(n+1).
 * https://oeis.org/A370265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = v₂( prime(i+j+1) – prime(i+1) ), where (i,j) are the
 *            antidiagonal coordinates (row and column) of n in the infinite
 *            rectangular array, read by antidiagonals
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* trailing 2‑adic valuation of a non‑zero seq_t value */
static inline seq_t trailing_zeros(seq_t x) {
    seq_t c = 0;
    /* x is always > 0 in this sequence */
    while ((x & 1u) == 0u) {
        c++;
        x >>= 1;
    }
    return c;
}

/* dynamic list of primes (1‑based indexing) */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* generate the next prime after the current list */
static uint64_t next_prime(void) {
    uint64_t cand;
    if (prime_count == 0) {
        cand = 2;
    } else {
        cand = prime_list[prime_count - 1] + 1;
        if (cand % 2 == 0) cand++;          /* start at odd */
    }
    for (;;) {
        bool ok = true;
        /* simple trial division with known primes */
        for (size_t k = 0; k < prime_count; ++k) {
            uint64_t p = prime_list[k];
            if (p * p > cand) break;
            if (cand % p == 0) {
                ok = false;
                break;
            }
        }
        if (ok) return cand;
        cand += 2;   /* only odd candidates after 2 */
    }
}

/* ensure that prime_list contains at least `needed` primes */
static void ensure_primes(size_t needed) {
    if (needed <= prime_count) return;

    if (!prime_list) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    for (; prime_count < needed; ++prime_count) {
        if (prime_count == prime_cap) {
            prime_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            if (!tmp) return;  /* allocation failed – leave the list as is */
            prime_list = tmp;
        }
        prime_list[prime_count] = next_prime();
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 0 */

    /* locate the antidiagonal that contains n */
    idx_t S = 1;           /* current diagonal length (1‑based) */
    idx_t cum = 0;         /* total elements before this diagonal */
    /* find S such that cum + S >= n */
    while (cum + S < n) {
        cum += S;
        S++;
    }

    /* position within this diagonal (0‑based) */
    idx_t offset = n - cum - 1;   /* 0‑based offset inside diagonal */
    idx_t i = offset + 1;         /* row index (1‑based) */
    idx_t j = S - offset;          /* column index (1‑based) */

    /* indices of the primes involved */
    idx_t idx1 = S + 2;   /* i + j + 1 = S + 2 */
    idx_t idx2 = i + 1;   /* i + 1 */

    /* make sure primes are available */
    size_t need_primes = (size_t)((idx1 > idx2) ? idx1 : idx2);
    ensure_primes(need_primes);

    if (prime_count < need_primes) return 0u; /* safety */

    uint64_t p1 = prime_list[idx1 - 1];
    uint64_t p2 = prime_list[idx2 - 1];
    seq_t diff = (seq_t) (p1 - p2);   /* positive difference */

    return trailing_zeros(diff);
}
