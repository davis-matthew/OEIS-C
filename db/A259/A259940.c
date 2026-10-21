/*
 * A259940  Let A={A005574(n)}, the set of all numbers n for which n^2+1 is
 *          prime. The sequence lists the number of decompositions A005574(n)
 *          = A005574(n1) + A005574(n2) for some n1, n2 and every
 *          A005574(n)>1.
 * https://oeis.org/A259940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   mod_mul_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes sequence A259940:
   a(1)=0.
   For n>1, a(n) is the number of unordered pairs (i,j) with
   1 ≤ i ≤ j < n such that
   A005574(i)+A005574(j) = A005574(n),
   where A005574(k) is the k‑th integer whose square plus one is prime.
   The function lazily generates the A005574 sequence up to the
   requested index and counts the decompositions by a two‑pointer scan.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include "../utils/mod_mul_u64.h"

/* ----- Miller‑Rabin primality test for 64‑bit integers ----- */

static inline uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1ULL) res = mod_mul_u64(res, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }
    static const uint64_t bases[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1ULL; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----- A005574 sequence handling ----- */
#define MAX_A_SIZE 20001   /* sufficient for n up to ~10 000 */

static uint64_t A_vals[MAX_A_SIZE];  /* A005574 values */
static size_t  gen = 0;              /* number of elements already generated */
static uint64_t next_candidate = 1;  /* integer to test for the property */

static void generate_up_to(idx_t n)
{
    while (gen < (size_t)n) {
        uint64_t cand = next_candidate;
        uint64_t test = cand * cand + 1ULL;  /* cand^2+1 < 2^64 for our limits */
        if (is_prime_u64(test)) {
            ++gen;
            A_vals[gen] = cand;
        }
        ++next_candidate;
    }
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined behaviour for n<1, but return 0 */

    if (n == 1) return (seq_t)0;   /* a(1)=0 by definition */

    generate_up_to(n);

    uint64_t target = A_vals[n];
    idx_t    l = 1;
    idx_t    r = n - 1;
    seq_t    count = 0;

    while (l <= r) {
        uint64_t sum = A_vals[l] + A_vals[r];
        if (sum == target) {
            ++count;
            ++l;
            --r;
        } else if (sum < target) {
            ++l;
        } else {
            --r;
        }
    }
    return count;
}
