/*
 * A353851  Number of integer compositions of n with all equal run-sums.
 * https://oeis.org/A353851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} tau(d) * (tau(d)-1)^(n/d-1)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 119, u64 n <= 245,
 *            u128 n <= 485, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow_seq.h"   /* pow_seq(seq_t base, int exp) */

static void init_primes(void);
static void div_enum(const unsigned long long *p, const int *e, size_t cnt,
                     size_t idx, unsigned long long cur, seq_t tau_cur,
                     unsigned long long n, seq_t *sum);

/* Prime sieve up to 10000 (sufficient for all n <= 10000) */
static unsigned long long primes_arr[20000];
static size_t primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void) {
    if (primes_ready) return;
    const size_t limit = 10000;
    bool comp[limit+1];
    memset(comp, 0, sizeof(comp));
    for (size_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            primes_arr[primes_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    comp[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* Recursive enumeration of divisors of n.
   Parameters:
     p   - array of distinct prime factors
     e   - corresponding exponents in the factorization of n
     cnt - number of distinct primes
     idx - current recursion depth
     cur - product of chosen prime powers (current divisor)
     tau_cur - product of (chosen_exponent+1) (i.e., tau(cur))
     n   - original number
     sum - pointer to accumulated sum a(n)
*/
static void div_enum(const unsigned long long *p, const int *e, size_t cnt,
                     size_t idx, unsigned long long cur, seq_t tau_cur,
                     unsigned long long n, seq_t *sum)
{
    if (idx == cnt) {
        /* term = tau_cur * (tau_cur-1)^(n/cur - 1) */
        unsigned long long tau_u64 = (unsigned long long)tau_cur;
        seq_t base = (seq_t)(tau_u64 - 1);
        int exp = (int)(n / cur - 1);   /* n/cur ≥ 1, so exp ≥ 0 */
        seq_t pow_term = pow_seq(base, exp);
        seq_t term     = tau_cur * pow_term;
        *sum += term;
        return;
    }
    unsigned long long prime = p[idx];
    int max_exp = e[idx];
    unsigned long long prod = 1;
    /* e = 0 case */
    div_enum(p, e, cnt, idx + 1, cur * prod,
             tau_cur * 1, n, sum);
    prod = 1;
    for (int exp = 1; exp <= max_exp; ++exp) {
        prod *= prime;
        div_enum(p, e, cnt, idx + 1, cur * prod,
                 tau_cur * (exp + 1), n, sum);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    init_primes();

    /* Factorize n */
    unsigned long long temp = (unsigned long long)n;
    unsigned long long p_buf[20];
    int e_buf[20];
    size_t fcnt = 0;

    for (size_t i = 0; i < primes_cnt && primes_arr[i] * primes_arr[i] <= temp; ++i) {
        unsigned long long pr = primes_arr[i];
        if (temp % pr == 0) {
            int cnt = 0;
            while (temp % pr == 0) {
                temp /= pr;
                ++cnt;
            }
            p_buf[fcnt] = pr;
            e_buf[fcnt] = cnt;
            ++fcnt;
        }
    }
    if (temp > 1) {   /* remaining prime factor */
        p_buf[fcnt] = temp;
        e_buf[fcnt] = 1;
        ++fcnt;
    }

    seq_t sum = (seq_t)0;
    div_enum(p_buf, e_buf, fcnt, 0, 1ULL, (seq_t)1,
             (unsigned long long)n, &sum);
    return sum;
}
