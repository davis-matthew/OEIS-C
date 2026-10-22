/*
 * A348950  a(n) = A348507(A276086(n)), where A348507(n) = A003959(n) - n,
 *          A003959 is multiplicative with a(p^e) = (p+1)^e, and A276086 gives
 *          the prime product form of primorial base expansion of n.
 * https://oeis.org/A348950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 207, u64 n <= 2096,
 *            u128 n <= 9865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = A348507(A276086(n))
   where A276086(n) gives the prime product of the primorial base
   expansion of n, and A348507(m) = A003959(m) – m.
   A003959 is multiplicative with a(p^e) = (p+1)^e.  The function
   works for any n ≥ 0 and uses only integer arithmetic on seq_t. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* first 128 primes – enough for n up to at least 1e12 */
static const uint64_t primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
    29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL,
    61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL,
    101ULL, 103ULL, 107ULL, 109ULL, 113ULL, 127ULL, 131ULL,
    137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL,
    173ULL, 179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL,
    211ULL, 223ULL, 227ULL, 229ULL, 233ULL, 239ULL, 241ULL,
    251ULL, 257ULL, 263ULL, 269ULL, 271ULL, 277ULL, 281ULL,
    283ULL, 293ULL, 307ULL, 311ULL, 313ULL, 317ULL, 331ULL,
    337ULL, 347ULL, 349ULL, 353ULL, 359ULL, 367ULL, 373ULL,
    379ULL, 383ULL, 389ULL, 397ULL, 401ULL, 409ULL, 419ULL,
    421ULL, 431ULL, 433ULL, 439ULL, 443ULL, 449ULL, 457ULL,
    461ULL, 463ULL, 467ULL, 479ULL, 487ULL, 491ULL, 499ULL,
    503ULL, 509ULL, 521ULL, 523ULL, 541ULL
};
static const size_t prime_cnt = sizeof primes / sizeof primes[0];

seq_t a(idx_t n)
{
    /* A276086(n) : prime product from primorial base digits  */
    idx_t  t   = n;          /* working copy of n   */
    seq_t  m   = 1;          /* product of p^digit  */
    seq_t  A   = 1;          /* product of (p+1)^digit */

    for (size_t i = 0; i < prime_cnt; ++i) {
        if (t == 0) break;                  /* representation complete */

        uint64_t p   = primes[i];
        idx_t    digit = t % p;              /* digit in this radix   */
        t = t / p;                          /* next higher radix     */

        /* compute p^digit and (p+1)^digit */
        seq_t p_pow   = 1;
        seq_t p1_pow  = 1;
        seq_t p_seq   = (seq_t)p;
        seq_t p1_seq  = (seq_t)(p + 1);

        for (idx_t j = 0; j < digit; ++j) {
            p_pow  *= p_seq;
            p1_pow *= p1_seq;
        }

        m *= p_pow;
        A *= p1_pow;
    }

    return A - m;     /* A348507(m) = A003959(m) – m */
}
