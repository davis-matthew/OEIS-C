/*
 * A272404  Primes p == 1 (mod 3) for which A261029(26*p) = 3.
 * https://oeis.org/A272404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th prime p == 1 (mod 3), p <= 676, with A261029(26*p) =
 *            3; A261029 counted by search over x<=y<=z, z>=x+1
 * Range:     u8 n <= 22, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A272404_L 17576

static int A272404_cnt[A272404_L + 1];
static int A272404_terms[200];
static int A272404_nterms = 0;
static bool A272404_ready = false;

static int64_t A272404_F(int64_t x, int64_t y, int64_t z)
{
    return x * x * x + y * y * y + z * z * z - 3 * x * y * z;
}

static void A272404_init(void)
{
    const int64_t L = A272404_L;
    for (int64_t x = 0; ; ++x) {
        if (A272404_F(x, x, x + 1) > L) break;
        for (int64_t y = x; ; ++y) {
            int64_t z = (y > x + 1) ? y : x + 1;
            int64_t f = A272404_F(x, y, z);
            if (f > L) break;
            while (f <= L) {
                A272404_cnt[f]++;
                int64_t inc = 3 * z * z + 3 * z + 1 - 3 * x * y;
                ++z;
                f += inc;
            }
        }
    }
    for (int p = 2; p <= 676; ++p) {
        bool pr = true;
        for (int d = 2; d * d <= p; ++d)
            if (p % d == 0) { pr = false; break; }
        if (pr && p % 3 == 1 && A272404_cnt[26 * p] == 3)
            A272404_terms[A272404_nterms++] = p;
    }
    A272404_ready = true;
}

seq_t a(idx_t n)
{
    if (!A272404_ready) A272404_init();
    if (n < 1 || n > (idx_t)A272404_nterms) return 0;
    return (seq_t)A272404_terms[n - 1];
}
