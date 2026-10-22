/*
 * A350013  Number of integer-sided triangles with one side having length n
 *          and an adjacent angle of 60 degrees.
 * https://oeis.org/A350013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 230, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) counts the integer solutions (x,y) to
   n^2 + x^2 – n·x = y^2 with x>0, y>0 and satisfying the
   triangle inequalities.  Using the identity
          (2y)^2 – (2x–n)^2 = 3n^2
   we write D = 2y – 2x + n, E = 2y + 2x – n, so that D·E = 3n^2.
   Every divisor D of 3n^2 gives a candidate solution:
      y = (D+E)/4 ,  x = (E–D+2n)/4          (E = 3n^2/D)
   The solution is valid iff D+E ≡ 0 (mod 4),  E–D+2n ≡ 0 (mod 4),
   and x>0.  We enumerate all divisors of 3n^2 by prime factorisation
   of n, produce the corresponding M = 3n^2, and count the admissible
   pairs.  Triangle inequalities are checked explicitly.  The
   implementation runs in O(d(3n^2)) time, which is below 2 s for
   n≤10000. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PRIME_LIMIT 10000   /* sufficient for n≤10000 */

/* global prime sieve */
static bool primes_initialised = false;
static uint32_t primes[MAX_PRIME_LIMIT];
static size_t prime_cnt = 0;

/* initialise primes up to MAX_PRIME_LIMIT */
static void init_primes(void)
{
    bool composite[MAX_PRIME_LIMIT + 1];
    memset(composite, 0, sizeof(composite));
    prime_cnt = 0;
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= MAX_PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    primes_initialised = true;
}

/* recursive divisor enumeration and counting */
static void enumerate_divisors(const uint32_t *pf,
                               const int *exp,
                               size_t idx, size_t num,
                               seq_t cur,
                               seq_t M,
                               seq_t *cnt,
                               seq_t n)
{
    if (idx == num) {
        seq_t d = cur;
        seq_t e = M / d;
        seq_t sum = d + e;
        if ((sum & 3U) == 0) {
            seq_t tmp = e - d + (seq_t)(2 * n);
            if ((tmp & 3U) == 0) {
                seq_t x = tmp >> 2;
                if (x > 0) {
                    seq_t y = sum >> 2;
                    if (x > 0 && y > 0) {
                        /* triangle inequalities */
                        if ((seq_t)n + x > y &&
                            (seq_t)n + y > x &&
                            x + y > (seq_t)n) {
                            (*cnt)++;
                        }
                    }
                }
            }
        }
        return;
    }
    seq_t p = pf[idx];
    seq_t val = 1;
    for (int k = 0; k <= exp[idx]; ++k) {
        enumerate_divisors(pf, exp, idx + 1, num, cur * val, M, cnt, n);
        val *= p;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise prime list once */
    if (!primes_initialised) init_primes();

    /* factor n */
    idx_t temp = n;
    uint32_t pf_n[20];
    int exp_n[20];
    size_t cnt_n = 0;
    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        int e = 0;
        while (temp % primes[i] == 0) {
            temp /= primes[i];
            ++e;
        }
        if (e) {
            pf_n[cnt_n] = primes[i];
            exp_n[cnt_n] = e;
            ++cnt_n;
        }
    }
    if (temp > 1) {
        pf_n[cnt_n] = (uint32_t)temp;
        exp_n[cnt_n] = 1;
        ++cnt_n;
    }

    /* build factorisation for M = 3 * n^2 */
    uint32_t pf_M[20];
    int exp_M[20];
    size_t cnt_M = 0;
    for (size_t i = 0; i < cnt_n; ++i) {
        pf_M[cnt_M] = pf_n[i];
        exp_M[cnt_M] = 2 * exp_n[i];
        ++cnt_M;
    }
    /* add prime 3 */
    bool found3 = false;
    for (size_t i = 0; i < cnt_M; ++i) {
        if (pf_M[i] == 3) {
            exp_M[i] += 1;
            found3 = true;
            break;
        }
    }
    if (!found3) {
        pf_M[cnt_M] = 3;
        exp_M[cnt_M] = 1;
        ++cnt_M;
    }

    /* compute M as idx_t, but it fits in 64 bits for n≤10000 */
    idx_t M = 3ULL * n * n;

    seq_t ans = 0;
    enumerate_divisors(pf_M, exp_M, 0, cnt_M, (seq_t)1, (seq_t)M, &ans, (seq_t)n);
    return ans;
}
