/*
 * A327831  Numbers m such that sigma(m)*tau(m) is a square but
 *          sigma(m)/tau(m) is not an integer.
 * https://oeis.org/A327831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth number m such that σ(m)*τ(m) is a perfect square
 *            but σ(m)/τ(m) is not an integer
 * Range:     u8 none, u16 n <= 1, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/*  Libraries and external utilities                           */
/* ------------------------------------------------------------ */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/isqrt_seq.h"   /* provides  isqrt_seq(seq_t)  */

/* ------------------------------------------------------------ */
/*  Prime sieve up to a fixed limit (enough for the target      */
/*  range of the sequence).  ----------------------------------- */
/* ------------------------------------------------------------ */

enum { PRIME_LIMIT = 1000000 };   /* 10⁶ is ample for m ≈ 10⁶ ß */

static uint32_t *primes = NULL;
static size_t    prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0) return;        /* already initialised */

    bool *is_composite = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);
    memset(is_composite, 0, (PRIME_LIMIT + 1) * sizeof(bool));

    size_t cap = 80000;                /* approximate number of primes ≤1e6 */
    primes = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt == cap) {    /* grow the array if needed */
                cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
        }
    }
    free(is_composite);
}

/* ------------------------------------------------------------ */
/*  Helper:  test whether σ * τ is a perfect square            */
/* ------------------------------------------------------------ */

static inline bool product_is_square(seq_t sigma, seq_t tau)
{
    seq_t prod = sigma * tau;
    seq_t root = isqrt_seq(prod);
    return root * root == prod;
}

/* ------------------------------------------------------------ */
/*  Storage for computed terms and last processed value        */
/* ------------------------------------------------------------ */

static seq_t *terms             = NULL;   /* cached terms */
static size_t terms_len          = 0;     /* number of cached terms */
static size_t terms_cap          = 0;     /* capacity of the array */
static idx_t  last_m_checked     = 0;     /* last m processed */

/* ------------------------------------------------------------ */
/*  Append a new term to the cache                            */
/* ------------------------------------------------------------ */

static void append_term(seq_t value)
{
    if (terms_len == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap << 1;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = value;
}

/* ------------------------------------------------------------ */
/*  Main sequence function:  a(n) (1‑based)                     */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* undefined for n==0 */

    init_primes();                         /* ensure prime table ready */

    /* generate terms until we have at least n terms */
    while (terms_len < n) {
        ++last_m_checked;
        uint64_t m = (uint64_t)last_m_checked;

        /* factorise m and compute σ(m) and τ(m) */
        seq_t sigma = 1;
        seq_t tau   = 1;
        uint64_t temp = m;

        for (size_t i = 0; i < prime_cnt &&
                (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {

            uint64_t p = primes[i];
            if (temp % p == 0) {
                uint8_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }

                /* σ multiplier for prime power p^e */
                seq_t p_pow = 1;
                for (uint8_t k = 0; k < e + 1; ++k)
                    p_pow *= (seq_t)p;
                seq_t sigma_mult = (p_pow - 1) / ((seq_t)p - 1);

                sigma *= sigma_mult;
                tau   *= (seq_t)(e + 1);
            }
        }

        if (temp > 1) {                    /* remaining prime factor */
            seq_t p = (seq_t)temp;
            sigma *= (1 + p);
            tau   *= 2;
        }

        /* check the two conditions */
        if ((sigma % tau) != 0) {
            if (product_is_square(sigma, tau))
                append_term((seq_t)m);
        }
    }

    return terms[n - 1];
}
