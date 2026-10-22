/*
 * A366230  Expansion of e.g.f. A(x,y) satisfying A(x,y) = 1 + x*A(x,y) *
 *          exp(x*y * A(x,y)), as a triangle read by rows.
 * https://oeis.org/A366230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle of coefficients T(r,c) of the
 *            exponential generating function A(x,y) satisfying A(x,y) = 1 +
 *            x*A(x,y)*exp(x*y*A(x,y)). Here r = row, c = column (0‑based),
 *            and T(r,c) = r! * binomial(r+1, c+1)/(r+1) * (r‑c)^c / c!. The
 *            mapping from the single index n to (r,c) is row...
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow_seq_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_seq_base.h"

/* --- Pre‑computed primes up to 20000 ---------------------------------- */
#define PRIME_LIMIT 20000
#define MAX_PRIMES 2300
static uint32_t primes[MAX_PRIMES];
static uint32_t primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    static bool is_comp[PRIME_LIMIT + 1];
    is_comp[0] = is_comp[1] = true;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) primes[primes_cnt++] = (uint32_t)i;
    primes_ready = true;
}

/* --- Prime exponent in n! --------------------------------------------- */
static uint64_t fact_prime_exp(idx_t n, uint32_t p)
{
    uint64_t exp = 0;
    while (n >= p) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* --- Prime exponent in n (n > 0) ------------------------------------- */
static uint64_t int_prime_exp(idx_t n, uint32_t p)
{
    uint64_t exp = 0;
    while (n % p == 0) {
        ++exp;
        n /= p;
    }
    return exp;
}

/* --- Map linear index to row r and column c --------------------------- */
static void index_to_row_col(idx_t n, idx_t *r, idx_t *c)
{
    idx_t rr = 0;
    while ((rr + 1) * (rr + 2) / 2 <= n) ++rr;
    *r = rr;
    *c = n - rr * (rr + 1) / 2;
}

/* --- Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_row_col(n, &r, &c);

    /* (r - c) = 0 and c > 0 => term is zero */
    if (r == c && c > 0) return (seq_t)0;

    init_primes();                     /* ensure primes table ready */

    seq_t result = 1;
    idx_t limit = r + 1;                /* primes needed up to r+1 */

    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint32_t p = primes[i];
        /* Exponents of prime p in the components of T(r,c) */
        int64_t e = 0;
        e += (int64_t)fact_prime_exp(r,     p);          /* n!           */
        e += (int64_t)fact_prime_exp(r + 1, p);          /* binom part    */
        e -= (int64_t)fact_prime_exp(c + 1, p);
        e -= (int64_t)fact_prime_exp(r - c, p);
        e -= (int64_t)int_prime_exp(r + 1, p);           /* denominator (r+1) */
        if (r > c)                                 /* (r-c)^c part  */
            e += (int64_t)c * (int64_t)int_prime_exp(r - c, p);
        e -= (int64_t)fact_prime_exp(c, p);          /* denominator c! */

        if (e > 0) {
            result *= pow_seq_base((seq_t)p, (uint64_t)e);
        }
        /* e should never be negative for valid parameters */
    }
    return result;
}
