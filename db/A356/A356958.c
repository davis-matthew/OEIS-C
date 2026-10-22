/*
 * A356958  Triangle read by rows: if n has weakly increasing prime indices
 *          (a,b,...,y,z) then row n is (b-a+1, ..., y-a+1, z-a+1).
 * https://oeis.org/A356958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
    Computes a(n) for OEIS A356958.
    The sequence is obtained by flattening the triangle:
        For each integer m ≥ 2, let its prime indices (in ascending order) be
        a,b,c,… (including multiplicities).  The row for m consists of
        (b‑a+1, c‑a+1, …).  Empty rows (m prime or 1) contribute nothing.
    a(n) is the n‑th element of this flattened list.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Parameters for the sieve ---------- */
#define LIMIT       300000UL       /* maximum m used (sufficient for ~10 000 terms) */
#define MAXPRIMES  30000UL        /* maximal number of primes below LIMIT */

/* ---------- Static data structures ---------- */
static idx_t spf[LIMIT + 1];          /* smallest prime factor of i */
static idx_t prime_index[LIMIT + 1];   /* index of prime i (1‑based), 0 if composite */
static idx_t primes[MAXPRIMES];       /* list of primes */
static size_t primes_cnt = 0;
static bool   sieve_done = false;

/* ---------- Sieve initialisation (linear) ---------- */
static void init_sieve(void)
{
    if (sieve_done) return;

    /* linear sieve building spf and primes */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_index[i] = ++primes_cnt;
            primes[primes_cnt - 1] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > LIMIT || p > spf[i]) break;
            spf[v] = p;
        }
    }
    sieve_done = true;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    init_sieve();

    idx_t count = 0;          /* number of entries seen so far */
    idx_t m = 2;              /* current integer whose row we process */

    while (1) {
        /* factor m into prime indices */
        idx_t temp = m;
        idx_t indices[32];    /* enough for m ≤ LIMIT (max exponent 18 for 2^18) */
        size_t idx_cnt = 0;

        while (temp > 1) {
            idx_t p = spf[temp];
            idx_t primeIdx = prime_index[p];
            do {
                temp /= p;
                indices[idx_cnt++] = primeIdx;
            } while (temp % p == 0);
        }

        if (idx_cnt > 1) {
            idx_t a0 = indices[0];
            for (size_t i = 1; i < idx_cnt; ++i) {
                seq_t val = (seq_t)(indices[i] - a0 + 1);
                ++count;
                if (count == n) return val;
            }
        }

        ++m;
    }
}
