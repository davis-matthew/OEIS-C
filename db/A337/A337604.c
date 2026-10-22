/*
 * A337604  Number of ordered triples of positive integers summing to n, any
 *          two of which have a common divisor > 1.
 * https://oeis.org/A337604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 629, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = # { (a,b,c) ∈ ℕ^3 : a+b+c = n, 
 *           gcd(a,b)>1, gcd(a,c)>1, gcd(b,c)>1 }.
 * Computation: precompute with a sieve which pairs have gcd>1
 * and then count ordered triples by a double loop over a,b.
 * Time complexity per call is O(n²) with a very small constant.
 * The sieve is built only once, for all values up to MAXN=10000.
 */
#include <stdint.h>

#define MAXN 10000
/* number of 64‑bit words needed to store a bitset of length MAXN+1 */
#define WORDS ((MAXN + 64) >> 6)

/* global static data: the truth table gcd(i,j)>1 for 0≤i,j≤MAXN */
static uint64_t gcd_gt1[MAXN + 1][WORDS];
static int   primes[1250];
static int   prime_cnt = 0;
static int   sieve_done = 0;

/* simple Euclidean gcd on unsigned integers */
static inline uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* create the table gcd_gt1 using a sieve of primes */
static void build_gcd_table(void)
{
    if (sieve_done) return;
    /* sieve of Eratosthenes */
    static unsigned char is_comp[MAXN + 1] = {0};
    int i, j;
    for (i = 2; i <= MAXN; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if ((int64_t)i * i <= MAXN)
                for (j = i * i; j <= MAXN; j += i)
                    is_comp[j] = 1;
        }
    }
    /* for every prime p, set bits for all pairs (x,y) with p|x and p|y */
    for (i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        uint64_t mask[WORDS] = {0};
        for (j = p; j <= MAXN; j += p) {
            int idx = j >> 6;
            mask[idx] |= 1ULL << (j & 63);
        }
        for (j = p; j <= MAXN; j += p) {
            int w;
            for (w = 0; w < WORDS; ++w)
                gcd_gt1[j][w] |= mask[w];
        }
    }
    sieve_done = 1;
}

/* helper to check gcd(i,j)>1 using the precomputed table */
static inline int has_common_prime(uint64_t i, uint64_t j)
{
    return (gcd_gt1[i][j >> 6] >> (j & 63)) & 1;
}

/* --------------------------------------------------------------------- */
/* Public function required by the exercise */
seq_t a(idx_t n)
{
    /* values for n < 6 are 0 by definition */
    if (n < 6) return 0;
    /* build table on first use */
    build_gcd_table();
    /* cast to proper type for calculation */
    const idx_t N = (idx_t)n;
    seq_t count = 0;
    idx_t a;
    for (a = 2; a <= N - 4; ++a) {              /* a>=2, a+b+c=n, so a<=N-4  */
        idx_t maxb = N - a - 2;                /* b>=2, b<=maxb          */
        idx_t b;
        for (b = 2; b <= maxb; ++b) {
            idx_t c = N - a - b;               /* c>=2 automatically     */
            /* all three pairwise gcds must be >1 */
            if (has_common_prime(a, b) &&
                has_common_prime(a, c) &&
                has_common_prime(b, c)) {
                ++count;
            }
        }
    }
    return count;
}
