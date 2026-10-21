/*
 * A246918  The length of the shortest nontrivial integral cevian of an
 *          equilateral triangle of side n that divides an edge into two
 *          integral parts, or 0 if no such cevian exists.
 * https://oeis.org/A246918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   recurse_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the OEIS sequence A246918.
 *
 * For an equilateral triangle of side n, the length y of a cevian
 * satisfies the Diophantine equation
 *     x² – y² – n·x + n² = 0 .
 *
 * All integer solutions (x, y) with x > 0 and x ≠ n correspond to
 * cevians that split an edge into two integer parts.  The shortest
 * non‑trivial integral cevian length is the minimum y ≠ n over all
 * such solutions.  If no non‑trivial solution exists the value is 0.
 *
 * The algorithm factors 3·n² and checks all factor pairs
 * u·v = 3·n² with identical parity and (u+v) divisible by 4.
 * For each pair the associated y and x are computed; if x>0 and
 * x≠n and y≠n we keep the smallest y.
 *
 * Complexity: O(τ(3·n²)) where τ(m) is the divisor function.
 * For n up to about 10 000 this is easily below the time limit.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/recurse_divs.h"

#define MAX_PRIME 20000           /* enough for n ≤ 10 000 (√3·n ≤ 17320) */

/* -------- prime sieve (once) -------- */
static bool sieve[MAX_PRIME + 1];
static uint64_t primes[MAX_PRIME / 10];
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited)
        return;

    for (size_t i = 2; i <= MAX_PRIME; ++i)
        sieve[i] = true;

    for (size_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = false;
        }
    }

    for (size_t i = 2; i <= MAX_PRIME; ++i)
        if (sieve[i])
            primes[prime_cnt++] = i;

    primes_inited = true;
}

/* -------- factorization of a 64‑bit integer -------- */
static void factor_uint64(uint64_t n, uint64_t *pf, int *pe, size_t *cnt)
{
    init_primes();
    size_t c = 0;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[c] = p;
            pe[c] = e;
            ++c;
        }
    }

    if (tmp > 1) {
        pf[c] = tmp;
        pe[c] = 1;
        ++c;
    }

    *cnt = c;
}

/* -------- main sequence function -------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* safety, offset starts at 1 */

    uint64_t nn = (uint64_t)n;
    uint64_t M = 3ULL * nn * nn;           /* 3·n² fits 64‑bit for n up to ~10⁴ */

    /* factor n into primes */
    uint64_t pf[32];
    int pe[32];
    size_t pf_cnt = 0;
    factor_uint64(nn, pf, pe, &pf_cnt);

    /* build prime power list for 3·n²: exponents doubled, plus one for 3 */
    size_t total = pf_cnt;
    for (size_t i = 0; i < pf_cnt; ++i)
        pe[i] *= 2;                  /* double exponents of n for n² */

    bool found3 = false;
    for (size_t i = 0; i < pf_cnt; ++i) {
        if (pf[i] == 3) {
            pe[i] += 1;             /* additional factor of 3 */
            found3 = true;
            break;
        }
    }
    if (!found3) {                  /* prime 3 not present */
        pf[pf_cnt] = 3;
        pe[pf_cnt] = 1;
        ++pf_cnt;
    }

    seq_t best = 0;
    recurse_divs(0, 1ULL, pf, pe, pf_cnt, M, nn, &best);
    return best;
}
