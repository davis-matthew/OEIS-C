/*
 * A358242  Consider all invertible residues k mod n. For each such k, find
 *          the product of three primes p*q*r = k (mod n) with the smallest
 *          max {p, q, r}. Then a(n) is the largest such p over the considered
 *          k.
 * https://oeis.org/A358242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest minimal maximal prime appearing in a
 *            representation of each invertible residue k mod n as a triple
 *            product of primes p≤q≤r (mod n). The algorithm searches primes
 *            in increasing order, keeps all products of at most two primes,
 *            and records the first prime that appears as the maxi...
 * Range:     u8 n <= 2920, u16 n <= 9050, u32 n <= 9050, u64 n <= 9050,
 *            u128 n <= 9050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* small helper: gcd for idx_t                                         */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Euler totient: number of residues coprime to m                       */
static idx_t totient(idx_t m)
{
    if (m == 0) return 0;
    idx_t result = m;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/* ------------------------------------------------------------------ */
/* simple primality test for 64‑bit numbers (fast enough for <1e9)     */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                               */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)2;   /* a(1)=2 */

    /* allocate arrays to remember which residues are covered           */
    bool *covered   = (bool *)calloc((size_t)n, sizeof(bool));
    bool *pair_flag = (bool *)calloc((size_t)n, sizeof(bool));
    if (!covered || !pair_flag) {
        free(covered); free(pair_flag);
        return (seq_t)0;          /* unlikely, just a safety net          */
    }

    /* dynamic lists of residues of primes and of products of two primes */
    size_t prime_cap = 0, prime_sz = 0;
    idx_t *prime_res = NULL;           /* residues of primes                */
    size_t pair_cap  = 0, pair_sz  = 0;
    idx_t *pair_res  = NULL;           /* residues of products of two primes*/

    idx_t tot_to_cover = totient(n);
    idx_t covered_cnt  = 0;
    seq_t best = 0;                     /* will hold a(n)                  */

    uint64_t cand = 2;                  /* candidate primes                */

    while (1) {
        /* test primality */
        if (is_prime(cand)) {
            /* only primes coprime to n matter */
            if (gcd_idx(cand, n) == 1) {
                idx_t res = (idx_t)(cand % n);

                /* update pair residues involving this prime */
                for (size_t i = 0; i < prime_sz; ++i) {
                    idx_t r   = prime_res[i];
                    idx_t prod = (uint64_t)res * r % n;
                    if (!pair_flag[prod]) {
                        pair_flag[prod] = true;
                        if (pair_sz == pair_cap) {
                            pair_cap = pair_cap ? pair_cap << 1 : 1;
                            pair_res = (idx_t *)realloc(pair_res, pair_cap * sizeof(idx_t));
                        }
                        pair_res[pair_sz++] = prod;
                    }
                }

                /* prime times itself */
                idx_t prod = (uint64_t)res * res % n;
                if (!pair_flag[prod]) {
                    pair_flag[prod] = true;
                    if (pair_sz == pair_cap) {
                        pair_cap = pair_cap ? pair_cap << 1 : 1;
                        pair_res = (idx_t *)realloc(pair_res, pair_cap * sizeof(idx_t));
                    }
                    pair_res[pair_sz++] = prod;
                }

                /* form triples where this prime is the largest */
                for (size_t i = 0; i < pair_sz; ++i) {
                    idx_t tri = (uint64_t)res * pair_res[i] % n;
                    if (!covered[tri]) {
                        covered[tri] = true;
                        ++covered_cnt;
                        if ((seq_t)cand > best) best = (seq_t)cand;
                        if (covered_cnt == tot_to_cover) {
                            /* clean up before returning */
                            free(covered);
                            free(pair_flag);
                            free(prime_res);
                            free(pair_res);
                            return best;
                        }
                    }
                }

                /* store residue for future pair calculations */
                if (prime_sz == prime_cap) {
                    prime_cap = prime_cap ? prime_cap << 1 : 1;
                    prime_res = (idx_t *)realloc(prime_res, prime_cap * sizeof(idx_t));
                }
                prime_res[prime_sz++] = res;
            }
        }

        if (covered_cnt == tot_to_cover) break;

        /* next candidate prime */
        cand += (cand == 2ULL) ? 1 : 2;
    }

    /* fallback: clean up and return best found (should not happen)   */
    free(covered);
    free(pair_flag);
    free(prime_res);
    free(pair_res);
    return best;
}
