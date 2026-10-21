/*
 * A246279  Transpose of array A246278: A(row,1) = 2*row, and for col > 1,
 *          A(row,col) = A003961(A(row,col-1)).
 * https://oeis.org/A246279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = transpose of array A246278 read along anti‑diagonals.
 * Each cell (row, col) is 2*row, and each prime factor is replaced
 * by its next prime (col‑1) times. */

/* standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- small helper functions ---------- */

/* primality test (trial division) – fast enough for the small
 * numbers used in this sequence (prime arguments < 3·10⁴). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1ULL) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* next prime > n (prime sieve not required). */
static uint64_t next_prime(uint64_t n)
{
    if (n < 2) return 2;
    uint64_t p = n + 1;
    if ((p & 1ULL) == 0) p++;        /* start with odd number > n */
    while (!is_prime_uint64(p)) p += 2;
    return p;
}

/* integer power for seq_t – exponent ≥ 0. */
static seq_t pow_seq_local(seq_t base, int exp)
{
    seq_t result = 1;
    seq_t b      = base;
    int   e      = exp;
    while (e > 0) {
        if (e & 1) result *= b;
        if (e > 1) b *= b;
        e >>= 1;
    }
    return result;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n < 1 – safety */

    /* find t such that t*(t+1)/2 ≥ n */
    idx_t t = 1;
    while (1) {
        __uint128_t tri = (__uint128_t)t * (t + 1) / 2;
        if (tri >= n) break;
        ++t;
    }

    /* previous triangular number T(t-1) = (t-1)*t/2 */
    idx_t prev = (idx_t)(((__uint128_t)(t - 1) * t) / 2);

    idx_t offset = n - prev - 1;       /* position inside diagonal (0‑based) */
    idx_t i = 1 + offset;              /* row (1‑based) */
    idx_t j = t - offset;              /* column (1‑based) */

    /* factor 2*i */
    uint64_t base = 2 * i;
    uint64_t primes[16];
    unsigned  exps[16];
    size_t nf = 0;

    uint64_t tmp = base;
    if ((tmp & 1ULL) == 0) {           /* factor 2 */
        unsigned e = 0;
        while ((tmp & 1ULL) == 0) { tmp >>= 1; ++e; }
        primes[nf] = 2;  exps[nf] = e;  ++nf;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[nf] = p;  exps[nf] = e;  ++nf;
        }
    }
    if (tmp > 1) {                     /* remaining prime factor */
        primes[nf] = tmp;  exps[nf] = 1;  ++nf;
    }

    /* apply A003961 (j‑1) times: shift every prime to its successor */
    if (j > 1) {
        for (idx_t step = 0; step < j - 1; ++step)
            for (size_t k = 0; k < nf; ++k)
                primes[k] = next_prime(primes[k]);
    }

    /* rebuild the value from shifted primes and exponents */
    seq_t result = 1;
    for (size_t k = 0; k < nf; ++k) {
        seq_t p  = (seq_t)primes[k];
        seq_t pw = pow_seq_local(p, (int)exps[k]);
        result *= pw;
    }

    return result;
}
