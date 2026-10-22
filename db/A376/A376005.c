/*
 * A376005  For integers >=2, the number of integer solutions to
 *          sqrt((n^2-c)*b^2 + c*(b+1)^2) where b and c are positive integers
 *          and c < n^2.
 * https://oeis.org/A376005
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n-1} (d_odd(k·(n-k)) – 1), where d_odd(m)
 *            denotes the number of odd divisors of m
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pre‑computed list of odd primes up to 10000 (sufficient for n≤10000) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool primes_ready = false;
static uint16_t prime_list[1229];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_ready) return;
    const size_t LIM = 10000;
    uint8_t is_p[LIM + 1];
    memset(is_p, 1, sizeof(is_p));
    is_p[0] = is_p[1] = 0;
    for (size_t i = 2; i * i <= LIM; ++i) {
        if (is_p[i]) {
            for (size_t j = i * i; j <= LIM; j += i)
                is_p[j] = 0;
        }
    }
    prime_cnt = 0;
    for (size_t i = 2; i <= LIM; ++i) {
        if (is_p[i]) {
            prime_list[prime_cnt++] = (uint16_t)i;
        }
    }
    primes_ready = true;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    init_primes();
    seq_t ans = 0;

    for (idx_t k = 1; k < n; ++k) {
        seq_t x = (seq_t)k * (seq_t)(n - k);   /* k * (n-k)  */
        /* strip all factors of 2 to obtain the odd part */
        while ((x & (seq_t)1) == 0) x >>= 1;

        seq_t odd_divs = 1;
        seq_t temp = x;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            seq_t p = (seq_t)prime_list[pi];
            if (p * p > temp) break;
            if (temp % p == 0) {
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                odd_divs *= (seq_t)(exp + 1);
            }
        }
        if (temp > 1) odd_divs *= (seq_t)2;  /* remaining prime factor */

        ans += odd_divs - (seq_t)1;
    }
    return ans;
}
