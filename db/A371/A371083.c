/*
 * A371083  Nonsquarefree numbers that are not divisible by p^p for any prime
 *          p.
 * https://oeis.org/A371083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 7472, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A371083:
   Nonsquarefree numbers that are not divisible by p^p for any prime p. */

/* include the standard C headers; the build supplies seq_t and idx_t */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------- */
/* Helper : generate all primes up to a given limit once          */
/* -------------------------------------------------------------- */
static uint64_t *global_primes = NULL;
static size_t   global_primes_cnt = 0;

/* simple sieve up to LIMIT */
static void init_primes(void)
{
    if (global_primes != NULL)
        return;

    /* For n up to ~10⁴ the 1‑st few thousand terms are well below 2·10⁵,   */
    /* but we use a generous limit to stay safe for all accepted builds. */
    const uint64_t LIMIT = 2000000ULL;
    uint8_t *composite = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!composite)
        return;
    composite[0] = composite[1] = 1;
    for (uint64_t i = 2; i * i <= LIMIT; ++i)
        if (!composite[i])
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                composite[j] = 1;

    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!composite[i])
            ++cnt;

    global_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!global_primes) {
        free(composite);
        return;
    }
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!composite[i])
            global_primes[idx++] = i;
    global_primes_cnt = cnt;

    free(composite);
}

/* -------------------------------------------------------------- */
/* Helper : test one integer for the required property           */
/* -------------------------------------------------------------- */
static bool is_valid_candidate(uint64_t m)
{
    bool has_square = false;
    uint64_t temp = m;

    /* trial division by pre‑computed primes up to sqrt(temp)        */
    for (size_t pi = 0; pi < global_primes_cnt; ++pi) {
        uint64_t p = global_primes[pi];
        if (p * p > temp)
            break;
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp >= 2) {
            has_square = true;
            /* p^p divides m ?  (prevent as per definition)              */
            if (exp >= (int)p)
                return false;
        }
    }

    /* Any remaining prime factor has exponent 1; no further checks     */
    if (!has_square)          /* squarefree -> not wanted */
        return false;

    return true;
}

/* -------------------------------------------------------------- */
/* Main function                                                */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* term 0 is undefined; return 0 for safety */

    /* Static storage for the sequence computed on demand            */
    static seq_t *terms      = NULL;
    static size_t terms_len  = 0;
    static size_t terms_cap  = 0;
    static uint64_t current_candidate = 9;

    if (terms_len < (size_t)n) {
        init_primes();                /* ensure prime list ready */

        while (terms_len < (size_t)n) {
            if (is_valid_candidate(current_candidate)) {
                /* grow the terms array if needed */
                if (terms_len == terms_cap) {
                    terms_cap = terms_cap ? terms_cap * 2 : 16;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)current_candidate;
            }
            ++current_candidate;
        }
    }
    return terms[n - 1];
}
