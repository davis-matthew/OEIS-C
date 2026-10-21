/*
 * A275083  Positive integers congruent to 0 or 1 modulo 4 that cannot be
 *          written as x^3 + y^2 + z^2 with x,y,z nonnegative integers.
 * https://oeis.org/A275083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A275083: numbers ≡ 0 or 1 (mod 4) that cannot be written
   as x³ + y² + z² (x,y,z ≥ 0).  The implementation searches
   successive candidates and keeps a memoised array of found terms. */

/*---------------------------------------------------------------------*/
/* Standard headers – available in C11 and C++17                       */
#include <stdlib.h>   /* malloc, free, realloc, memset */
#include <stdint.h>   /* uint32_t, uint8_t, etc.          */
#include <stdbool.h>  /* bool, true, false                */

/*---------------------------------------------------------------------*/
/* Prime table – used for the sum‑of‑two‑squares test.                 */
static uint32_t *primes   = NULL;   /* primes up to PRIMELIMIT          */
static size_t   prime_cnt = 0;      /* number of primes stored          */
#define PRIMELIMIT 1000000U       /* should cover sqrt of all m tested */

/*---------------------------------------------------------------------*/
/* Sequence cache – stores the first terms already generated.          */
static seq_t *seq_arr = NULL;   /* memoised sequence values          */
static size_t seq_size = 0;     /* number of values stored           */
static size_t seq_cap  = 0;     /* currently allocated slots         */

/*---------------------------------------------------------------------*/
/* Current integer being examined: only numbers ≡0 or 1 (mod 4)       */
static idx_t curM = 0;          /* last integer examined              */

/*---------------------------------------------------------------------*/
/* Prime sieve – builds `primes` up to PRIMELIMIT.                     */
static void init_primes(void)
{
    uint8_t *mark = (uint8_t *)malloc(PRIMELIMIT + 1);
    if (!mark) return;   /* if allocation fails, we will never test sums */
    memset(mark, 0, PRIMELIMIT + 1);

    for (uint32_t i = 2; i * i <= PRIMELIMIT; ++i) {
        if (!mark[i]) {
            for (uint32_t j = i * i; j <= PRIMELIMIT; j += i)
                mark[j] = 1;
        }
    }

    /* Count primes first */
    prime_cnt = 0;
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i)
        if (!mark[i]) prime_cnt++;

    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) { free(mark); return; }

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i)
        if (!mark[i]) primes[idx++] = i;

    free(mark);
}

/*---------------------------------------------------------------------*/
/* Checks whether n is representable as a sum of two squares.           */
static bool is_sum_two_squares(seq_t n)
{
    if (n == 0) return true;
    seq_t temp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t  p  = primes[i];
        seq_t     pS = (seq_t)p;
        seq_t     pSq = pS * pS;
        if (pSq > temp) break;

        if (temp % pS == 0) {
            unsigned cnt = 0;
            while (temp % pS == 0) {
                temp /= pS;
                ++cnt;
            }
            if ((p & 3U) == 3U && (cnt & 1U)) return false;
        }
    }

    /* Remaining factor (if any) is a prime > sqrt(original).  */
    if (temp > 1) {
        if ((temp % 4U) == 3U) return false;
    }
    return true;
}

/*---------------------------------------------------------------------*/
/* Checks whether m can be written as x³ + y² + z² (x,y,z ≥ 0)           */
static bool is_representable(seq_t m)
{
    /* Try all cubes x³ ≤ m.  x is small because x³ grows fast. */
    seq_t x = 0;
    for (;;) {
        seq_t cube = (x * x) * x;
        if (cube > m) break;
        seq_t remaining = m - cube;
        if (is_sum_two_squares(remaining)) return true;
        ++x;
        /* Prevent overflow of x for very large m */
        if (x > 0xFFFFFFFFUL) break;
    }
    return false;
}

/*---------------------------------------------------------------------*/
/* Fills the memoised array with at least n terms.                     */
static void ensure_seq_size(idx_t n)
{
    if (!primes) init_primes();
    if (!primes) return;   /* if prime sieve failed, we cannot continue */

    while (seq_size < (size_t)n) {
        /* advance to the next integer ≡0 or 1 (mod 4) */
        do { ++curM; } while ((curM & 3) > 1);

        seq_t m = (seq_t)curM;   /* truncate if seq_t is smaller */
        if (!is_representable(m)) {
            /* store as a new term */
            if (seq_size == seq_cap) {
                seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!seq_arr) return;   /* allocation failed */
            }
            seq_arr[seq_size++] = m;
        }
    }
}

/*---------------------------------------------------------------------*/
/* External function required by the test harness.                     */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_primes();
        seq_arr = NULL;
        seq_size = 0;
        seq_cap = 0;
        curM = 0;
        initialized = true;
    }

    ensure_seq_size(n);
    return seq_arr[n - 1];
}
