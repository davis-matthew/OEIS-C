/*
 * A390685  a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(4*n-2*k-2,n-2*k).
 * https://oeis.org/A390685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (-1)^k * binomial(4*n-2*k-2, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ---------- small prime sieve (up to 40000) ------------------ */
static uint32_t *primes = NULL;          /* array of primes  */
static size_t   primes_cnt = 0;          /* number of primes  */

static void init_primes(void)
{
    if (primes != NULL)
        return;                          /* already initialized */

    const idx_t LIM = 40000;
    bool *is_comp = (bool*)malloc((LIM + 1) * sizeof(bool));
    if (!is_comp)
        return;                          /* unlikely by contract */

    memset(is_comp, 0, (LIM + 1) * sizeof(bool));

    for (idx_t i = 2; i * i <= LIM; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= LIM; j += i)
                is_comp[j] = true;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIM; ++i)
        if (!is_comp[i]) ++cnt;

    /* store primes */
    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= LIM; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;

    primes_cnt = cnt;
    free(is_comp);
}

/* ---------- binomial via prime exponents --------------------- */
static seq_t binom_prime(idx_t n, idx_t k)
{
    if (k > n - k) /* symmetry */
        k = n - k;

    seq_t res = 1;

    if (k == 0)
        return 1;      /* trivial case */

    /* ensure primes are ready */
    init_primes();

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = primes[pi];
        if (p > n)
            break;

        idx_t exp = 0;
        idx_t t;

        /* exponent in n!  */
        t = n;
        while (t) {
            t /= p;
            exp += t;
        }

        /* subtract exponent in k! */
        t = k;
        while (t) {
            t /= p;
            exp -= t;
        }

        /* subtract exponent in (n-k)! */
        t = n - k;
        while (t) {
            t /= p;
            exp -= t;
        }

        /* multiply prime^exp into result */
        for (idx_t e = 0; e < exp; ++e)
            res *= p;
    }

    return res;
}

/* ---------- term update (term * num / den without overflow) ---- */
static seq_t mul_div_by_gcd(seq_t term, seq_t num, seq_t den)
{
    seq_t g;

    g = gcd(term, den);
    term /= g;
    den /= g;

    g = gcd(num, den);
    num /= g;
    den /= g;          /* should become 1 */

    /* now term * num fits in seq_t */
    return term * num;
}

/* ---------- main sequence function ---------------------------- */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 1;      /* a(0) */

    /* first term: C(4n-2, n) */
    idx_t a_val = 4 * n - 2u;   /* 4n-2k-2 with k=0 */
    idx_t b_val = n;            /* n-2k= n */
    seq_t term   = binom_prime(a_val, b_val);
    seq_t sum    = term;

    idx_t maxk = n / 2u;
    for (idx_t k = 1; k <= maxk; ++k) {
        /* ratio: (b*(b-1))/(a*(a-1))   with current a_val, b_val  */
        seq_t num = (seq_t)b_val * (b_val - 1u);
        seq_t den = (seq_t)a_val * (a_val - 1u);

        term = mul_div_by_gcd(term, num, den);

        /* add or subtract based on parity of k */
        if (k & 1u)
            sum -= term;
        else
            sum += term;

        /* prepare for next k */
        a_val -= 2u;
        b_val -= 2u;
    }

    return sum;
}
