/*
 * A332802  a(n) is the smallest q such that the number of nonnegative k <= q,
 *          possessing the property that k + k*q - q is a square, is equal to
 *          2^n.
 * https://oeis.org/A332802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 4*Π_{i=1}^{n-1} p_i – 1 for n ≥ 2; a(0)=0, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *prod = NULL;      /* prod[k] = product of first k primes (prod[0]=1) */
static size_t prod_cap = 0;
static size_t prod_len = 0;     /* number of stored products */

static uint64_t *primes = NULL; /* list of found primes */
static size_t primes_cap = 0;
static size_t primes_len = 0;   /* number of stored primes */

static uint64_t candidate = 2;  /* next candidate prime to test */

/* Ensure prod array has at least 'need' elements (prod[0]..prod[need-1])   */
static void ensure_prod_storage(size_t need)
{
    if (prod_len >= need) return;
    size_t new_cap = prod_cap ? prod_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    prod = (seq_t*)realloc(prod, new_cap * sizeof(seq_t));
    prod_cap = new_cap;
}

/* Ensure primes array has enough capacity and compute missing primes      */
static void ensure_primes(size_t need)
{
    while (primes_len < need) {
        /* test if 'candidate' is prime */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len &&
             (uint64_t)primes[i] * primes[i] <= candidate; ++i) {
            if (candidate % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            /* add prime to array */
            if (primes_len >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                primes = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;

            /* update product array with new product */
            ensure_prod_storage(prod_len + 1);
            prod[prod_len] = prod[prod_len - 1] * (seq_t)candidate;
            prod_len++;
        }
        /* advance candidate to next odd number after 2 */
        candidate = (candidate == 2 ? 3 : candidate + 2);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise static arrays on first call */
    if (prod_cap == 0) {
        prod_cap = 8;
        prod = (seq_t*)malloc(prod_cap * sizeof(seq_t));
        prod[0] = (seq_t)1;   /* product of zero primes */
        prod_len = 1;

        primes_cap = 64;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes_len = 0;

        candidate = 2;
    }

    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    /* Need product of first (n-1) primes */
    size_t need = (size_t)(n - 1);
    ensure_primes(need);
    seq_t p = prod[need];
    return p * ((seq_t)4) - ((seq_t)1);
}
