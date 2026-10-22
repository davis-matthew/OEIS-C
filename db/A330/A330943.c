/*
 * A330943  Matula-Goebel numbers of singleton-reduced rooted trees.
 * https://oeis.org/A330943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A330943 – Matula‑Goebel numbers of singleton‑reduced rooted trees
    The number n belongs to the set iff
        n = 1
        or (for every prime divisor p of n, let m = index of p in the prime list;
            m is in the set, and at least one such m is not prime).
    Membership is computed by iterating through n from 2 upwards, using a
    linear sieve to obtain the smallest prime factor and the prime index of
    each prime.  The sequence values are the sorted list of integers that
    satisfy the membership test.  The first 24 terms are
    1,2,4,6,7,8,12,13,14,16,18,19,21,24,26,28,32,34,36,37,38,39,42,43. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
   Adjustable limit – large enough to generate at least 10 000 terms
   (the typical test range).  The build may override MAX_SEQ_LIMIT.
   -------------------------------------------------------------------- */
#if defined(MAX_SEQ_LIMIT)
#  define LIMIT  MAX_SEQ_LIMIT
#else
#  define LIMIT  2000000
#endif

/* --------------------------------------------------------------------
   Global static data used for memoisation.  They are initialised
   lazily on the first call to a().
   -------------------------------------------------------------------- */
static bool        *member = NULL;   /* is n in the sequence?               */
static bool        *is_prime = NULL; /* primality of numbers up to LIMIT    */
static uint32_t    *prime_idx = NULL;/* index of prime p (1‑based)          */
static uint32_t    *spf = NULL;      /* smallest prime factor of i          */
static idx_t      *term_list = NULL; /* the sequence values, 1‑based         */
static size_t      term_count = 0;    /* number of found sequence terms      */
static bool        initialized = false;

/* --------------------------------------------------------------------
   Linear sieve that produces spf[], prime_idx[], and is_prime[].
   -------------------------------------------------------------------- */
static void init(void)
{
    /* Allocate basic arrays ------------------------------------------------*/
    member     = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    is_prime   = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    prime_idx  = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    spf        = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    term_list  = (idx_t*) malloc((LIMIT + 1) * sizeof(idx_t));
    if (!member || !is_prime || !prime_idx || !spf || !term_list)
        exit(1);
    memset(member,   0, (LIMIT + 1) * sizeof(bool));
    memset(is_prime, 0, (LIMIT + 1) * sizeof(bool));
    memset(prime_idx,0, (LIMIT + 1) * sizeof(uint32_t));
    memset(spf,      0, (LIMIT + 1) * sizeof(uint32_t));

    /* Allocate temporary prime list ------------------------------------*/
    size_t  est_primes = LIMIT / 10 + 10;          /* rough upper bound */
    uint32_t *primes  = (uint32_t*)malloc(est_primes * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t  pcnt = 0;                               /* current number of primes */

    /* Linear sieve --------------------------------------------------------*/
    for (idx_t i = 2; i <= (idx_t)LIMIT; ++i) {
        if (spf[(size_t)i] == 0) {                /* i is prime  */
            spf[(size_t)i]   = (uint32_t)i;
            prime_idx[(size_t)i] = (uint32_t)(pcnt + 1);  /* 1‑based index */
            primes[pcnt++] = (uint32_t)i;
            is_prime[(size_t)i] = true;
        } else
            is_prime[(size_t)i] = false;

        for (size_t j = 0; j < pcnt; ++j) {
            uint32_t p = primes[j];
            idx_t x = i * (idx_t)p;
            if (x > (idx_t)LIMIT) break;
            spf[(size_t)x] = p;
            if (i % p == 0) break;
        }
    }
    free(primes);

    /* --------------------------------------------------------------------
       Determine membership for each number up to LIMIT
       --------------------------------------------------------------------*/
    member[1] = true;    /* base case */

    for (idx_t n = 2; n <= (idx_t)LIMIT; ++n) {
        bool ok = true;
        bool anyComposite = false;
        idx_t temp = n;
        while (temp > 1) {
            uint32_t p = spf[(size_t)temp];
            uint32_t m = prime_idx[(size_t)p];     /* index of prime p */
            if (!member[(size_t)m]) {              /* m not in set */
                ok = false;
                break;
            }
            if (!is_prime[(size_t)m])              /* m composite */
                anyComposite = true;
            while (temp % p == 0) temp /= p;
        }
        member[(size_t)n] = ok && anyComposite;
    }

    /* --------------------------------------------------------------------
       Build increasing list of sequence terms
       --------------------------------------------------------------------*/
    for (idx_t n = 1; n <= (idx_t)LIMIT; ++n) {
        if (member[(size_t)n]) {
            term_list[term_count++] = n;
            if (term_count == LIMIT) break;        /* safety guard */
        }
    }
}

/* --------------------------------------------------------------------
   Public sequence function
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!initialized) {
        init();
        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */

    if (n > term_count) {
        /* Requested index beyond pre‑computed range – return 0 as error */
        return (seq_t)0;
    }

    return (seq_t)term_list[n - 1];
}
