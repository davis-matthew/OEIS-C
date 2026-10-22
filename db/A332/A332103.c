/*
 * A332103  Numbers not of the form floor(p/4) + 1, where p is a prime.
 * https://oeis.org/A332103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332103: Numbers not of the form floor(p/4)+1, where p is prime. */
/* The function returns the n-th such number (1‑based).  The algorithm
   generates primes incrementally, marks the integers floor(p/4)+1
   in a dynamic bit array, and scans for the n-th missing number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---- dynamic prime list --------------------------------------------- */
static uint64_t *prime_list      = NULL;   /* list of found primes    */
static size_t   prime_cap       = 0;      /* allocated capacity      */
static size_t   prime_cnt       = 0;      /* number of primes stored */
static uint64_t next_prime_cand  = 3;      /* next odd candidate     */

/* enlarge the prime list buffer */
static void primes_grow(size_t new_cap)
{
    if (new_cap <= prime_cap) return;
    size_t old_cap = prime_cap;
    prime_cap = new_cap;
    prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    if (!prime_list) abort();
    /* initialise new part to zero (not needed but safe) */
    if (old_cap < prime_cap)
        memset(prime_list + old_cap, 0, (prime_cap - old_cap) * sizeof(uint64_t));
}

/* add a newly found prime, mark its floor(p/4)+1 value */
static void add_prime(uint64_t p);

/* ---- dynamic marker for values floor(p/4)+1 --------------------------- */
static unsigned char *s_mark      = NULL;   /* 1 if value present, 0 otherwise */
static size_t   s_cap            = 0;      /* allocated size (indices 0 .. s_cap-1) */

/* enlarge the marker buffer and zero the new part */
static void s_grow(size_t new_cap)
{
    if (new_cap <= s_cap) return;
    size_t old_cap = s_cap;
    s_cap = new_cap;
    s_mark = (unsigned char *)realloc(s_mark, s_cap);
    if (!s_mark) abort();
    if (old_cap < s_cap)
        memset(s_mark + old_cap, 0, s_cap - old_cap);
}

/* ---- primes generation ------------------------------------------------ */
static void generate_primes_up_to(uint64_t limit)
{
    /* generate all primes ≤ limit, starting from next_prime_cand */
    while (next_prime_cand <= limit) {
        uint64_t cand = next_prime_cand;
        next_prime_cand += 2;          /* skip even numbers */

        /* trial division using known primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            add_prime(cand);
        }
    }
}

/* add p to the prime list and mark floor(p/4)+1 */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) primes_grow((prime_cap == 0) ? 8 : prime_cap * 2);
    prime_list[prime_cnt++] = p;

    /* compute q = floor(p/4)+1 */
    uint64_t q = p / 4 + 1;
    s_grow(q + 1);
    s_mark[q] = 1;
}

/* ---- initialisation --------------------------------------------------- */
static bool init_done = false;
static void ensure_init(void)
{
    if (init_done) return;
    /* start with prime 2 */
    primes_grow(8);
    prime_list[0] = 2;
    prime_cnt = 1;
    next_prime_cand = 3;

    /* marker array: we need index 0 at least */
    s_grow(1);
    init_done = true;
}

/* ---- main function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_init();

    idx_t missing = 0;          /* number of found missing integers */
    for (idx_t x = 0; ; ++x) {
        /* ensure all primes ≤ 4*x are processed (they may produce values ≤ x) */
        generate_primes_up_to(4 * (uint64_t)x);

        bool present = (x < (idx_t)s_cap && s_mark[x]);
        if (!present) {
            ++missing;
            if (missing == n) return (seq_t)x;
        }
    }
}
