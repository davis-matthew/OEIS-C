/*
 * A282100  a(n) is the least number of successive prime numbers modulo 100
 *          required such that their sum is at least 100.
 * https://oeis.org/A282100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least length of a block of successive primes (mod 100)
 *            whose cumulative residue sum is at least 100
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised; `next_prime` generates primes on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global static state */
static uint32_t *prime_list = NULL;   /* array of primes generated      */
static size_t   prime_cap  = 0;       /* allocated capacity of prime_list   */
static size_t   prime_len  = 0;       /* number of primes stored           */
static uint64_t candidate  = 2;       /* next candidate for primality test */

static seq_t *blocks = NULL;          /* lengths of completed blocks     */
static size_t  blocks_cap = 0;        /* capacity of the blocks array   */
static size_t  blocks_len = 0;        /* number of blocks stored        */

/* Forward declaration of the prime generator */
static uint32_t next_prime(void);

/* --------------------------------------------------------------------- */
/* a(n)  –  returns the nth term (1‑based)                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure we have computed blocks up to index n (1‑based) */
    while (blocks_len < (size_t)n)
    {
        unsigned int sum = 0;
        unsigned int cnt = 0;

        /* Accumulate residues until sum ≥ 100 */
        while (sum < 100)
        {
            uint32_t p = next_prime();          /* next prime */
            unsigned int r = p % 100;           /* residue mod 100 */
            sum += r;
            ++cnt;
        }

        /* Store block length */
        if (blocks_len == blocks_cap)
        {
            size_t new_cap = blocks_cap == 0 ? 4 : blocks_cap * 2;
            seq_t *tmp = (seq_t *)realloc(blocks, new_cap * sizeof(seq_t));
            if (!tmp) abort();                  /* memory exhausted */
            blocks = tmp;
            blocks_cap = new_cap;
        }
        blocks[blocks_len++] = (seq_t)cnt;       /* cast fits seq_t */
    }

    return blocks[(size_t)n - 1];
}

/* --------------------------------------------------------------------- */
/* next_prime() – Generates the next prime and appends it to prime_list  */
/* --------------------------------------------------------------------- */
static uint32_t next_prime(void)
{
    for (;;)
    {
        uint32_t p = (uint32_t)candidate;

        /* Ensure capacity of prime_list */
        if (prime_len == prime_cap)
        {
            size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
            uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
            if (!tmp) abort();
            prime_list = tmp;
            prime_cap  = new_cap;
        }

        /* Test divisibility by known primes up to sqrt(p) */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i)
        {
            uint32_t q = prime_list[i];
            if ((uint64_t)q * (uint64_t)q > p) break;
            if (p % q == 0) { is_prime = false; break; }
        }

        /* Store prime if identified */
        if (is_prime)
        {
            prime_list[prime_len++] = p;
            candidate = (candidate == 2) ? 3 : candidate + 2;
            return p;
        }

        /* Advance candidate: skip even numbers after 2 */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }
}
