/*
 * A275515  Table read by rows: list of prime triples of the form (p, p+2,
 *          p+6).
 * https://oeis.org/A275515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 549, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for prime triples of the form (p, p+2, p+6),
   reading the table row‑wise.  a(1)=5, a(2)=7, a(3)=11, a(4)=11, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*---------------------------------------------------------------------*/
/*   Miller–Rabin primality test for 64‑bit integers (deterministic)
   Bases 2,3,5,7,11,13 are sufficient for n < 2^64.                       */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static inline bool is_prime(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    if (n % 3 == 0)     return n == 3;

    static const uint64_t bases[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};

    /* Trial division by small primes to speed up */
    for (size_t i = 0; ; ++i) {
        uint64_t p = bases[i];
        if (p == 0) break;            // safety
        if (n == p)                 return true;
        if (n % p == 0)             return false;
    }

    /* Write n‑1 as d * 2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Miller–Rabin rounds using the same bases */
    for (size_t i = 0; ; ++i) {
        uint64_t a = bases[i];
        if (a == 0 || a >= n) break;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool witness = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { witness = false; break; }
        }
        if (witness) return false;
    }
    return true;
}

/*---------------------------------------------------------------------*/
/*   Storage for discovered triples: only their first element p.        */
static seq_t *triples = NULL;
static size_t triple_cap = 0;
static size_t triple_cnt = 0;

/*   Last odd candidate examined.  Initially 5 (the first base).      */
static uint64_t candidate = 5U;

/*---------------------------- grow array --------------------------------*/
static void grow_triples(void)
{
    size_t new_cap = (triple_cap == 0) ? 8 : triple_cap * 2;
    seq_t *tmp = (seq_t *)realloc(triples, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failure */
    triples = tmp;
    triple_cap = new_cap;
}

/*----------------------- ensure enough triples ------------------------*/
static void ensure_triples(size_t target_index)
{
    while (triple_cnt <= target_index) {
        /* Search for the next base p of a prime triple. */
        while (true) {
            if (is_prime(candidate) &&
                is_prime(candidate + 2) &&
                is_prime(candidate + 6)) {
                if (triple_cnt == triple_cap) grow_triples();
                triples[triple_cnt++] = (seq_t)candidate;
                break;
            }
            candidate += 2;           /* skip even numbers */
        }
        candidate += 2;               /* move to next odd after adding */
    }
}

/*---------------------------------------------------------------------*/
/*   Public function required by the harness.  External linkage.         */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n ≤ 0 */

    /* Index of the triple containing a(n) (0‑based). */
    size_t triple_index = (size_t)((n + 2) / 3) - 1;
    size_t offset = (size_t)((n - 1) % 3);

    ensure_triples(triple_index);

    seq_t p = triples[triple_index];
    if (offset == 0)   return p;
    if (offset == 1)   return p + 2;
    return p + 6;      /* offset == 2 */
}
