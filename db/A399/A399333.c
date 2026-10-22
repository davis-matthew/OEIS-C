/*
 * A399333  The binomial matrix of the noncomposite numbers (A008578). Square
 *          array read by ascending antidiagonals.
 * https://oeis.org/A399333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A399333 – Binomial matrix of the non‑composite numbers
 *
 *  Each element A(i ,j) (i ,j ≥ 0) is defined by
 *      A(i ,j) = Σ_{m=0..d} binomial(d ,m) · s^{d‑m} · P(p+m)
 *  where
 *      d = |i - j| ,
 *      p = min(i ,j) ,
 *      s =  1  if j ≤ i  (otherwise -1) ,
 *      P(0) = 1 ,  P(k) = k‑th prime   (k ≥ 1).
 *
 *  The sequence {a(n)} is obtained by reading the infinite square array
 *  A(i ,j) along ascending antidiagonals:  
 *  a(0)=A(0,0), a(1)=A(1,0), a(2)=A(0,1), a(3)=A(2,0), …
 *
 *  The implementation below computes a(n) directly from the closed
 *  formula for A(i ,j).  A small prime sieve is performed once
 *  to provide the primes needed for the non‑composite table.
 *
 *  All arithmetic is performed with seq_t, which may be a built‑in
 *  integral type or an arbitrary‑precision integer class.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime table -------------------------------------------------- */
static seq_t *prime_tbl = NULL;   /* 0‑based: prime_tbl[0] = 1, primed[1] = 2, ... */
static size_t prime_cnt   = 0;    /* number of elements in prime_tbl */

/* prime sieve limit – sufficient for n up to ≈10⁶ (k ≈ 1.4·10³)          */
#define PRIME_LIMIT 200000UL

static void init_prime_tbl(void)
{
    if (prime_tbl) return;                /* already initialised */

    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;
    for (idx_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    /* count primes */
    prime_cnt = 1;          /* for the leading 1 */
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    prime_tbl = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
    if (!prime_tbl) {
        free(is_prime);
        return;
    }

    prime_tbl[0] = (seq_t)1;   /* P(0) = 1 */
    size_t idx = 1;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) prime_tbl[idx++] = (seq_t)i;

    free(is_prime);
}

/* helper: return P(k) */
static inline seq_t noncomposite(idx_t k)
{
    if (!prime_tbl) init_prime_tbl();
    /* k < prime_cnt is assumed (true for all values used)      */
    return prime_tbl[k];
}

/* ---------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    /* 1. find diagonal index k with (k+1)(k+2)/2 > n  */
    idx_t k = 0;
    while (((k + 1) * (k + 2) / 2) <= n) ++k;

    /* 2. offset r on that diagonal (0‑based)          */
    idx_t r = n - k * (k + 1) / 2;

    /* 3. row and column of the required entry          */
    idx_t i = k - r;
    idx_t j = r;

    /* 4. compute A(i ,j) by the defining sum           */
    idx_t d = (i > j) ? (i - j) : (j - i);
    idx_t p = (i < j) ? i : j;
    seq_t s = (j <= i) ? ((seq_t)1) : ((seq_t)(-1));

    seq_t result = 0;
    seq_t bin    = 1;                 /* binomial(d ,0) */

    for (idx_t m = 0; m <= d; ++m) {
        seq_t prime_val = noncomposite(p + m);
        seq_t term      = bin * prime_val;

        /* apply sign factor s^{d-m}   */
        seq_t sign = 1;
        if (s == -1) {
            if ((d - m) & 1U)
                sign = -1;
        }
        result += term * sign;

        /* update binomial for next m   */
        if (m < d) {
            bin = bin * (d - m) / (m + 1);
        }
    }

    return result;
}
