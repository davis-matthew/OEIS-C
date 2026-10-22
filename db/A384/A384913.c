/*
 * A384913  The number of unordered factorizations of n into exponentially
 *          Fibonacci powers of primes (A115975).
 * https://oeis.org/A384913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} A003107(e) where A003107(e) = number of
 *            partitions of e into Fibonacci numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for uint64_t */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Pre‑computed values of A003107(e) for 0 ≤ e ≤ 63 */
static seq_t fib_part[64];
static int fib_part_init = 0;
static int fib_part_max_exp = 63;

static void init_fib_part(void)
{
    if (fib_part_init) return;

    int i, j;
    for (i = 0; i <= fib_part_max_exp; ++i) fib_part[i] = 0;
    fib_part[0] = 1;               /* 0 has one empty partition */

    /* Build unique Fibonacci numbers ≤ fib_part_max_exp: 1,2,3,5,… */
    int fibs[20];
    int fibcnt = 0;
    fibs[fibcnt++] = 1;
    int f1 = 1, f2 = 2;
    while (f2 <= fib_part_max_exp) {
        fibs[fibcnt++] = f2;
        int next = f1 + f2;
        f1 = f2;
        f2 = next;
    }

    /* Coin‑change DP to count partitions */
    for (i = 0; i < fibcnt; ++i) {
        int v = fibs[i];
        for (j = v; j <= fib_part_max_exp; ++j) {
            fib_part[j] = fib_part[j] + fib_part[j - v];
        }
    }
    fib_part_init = 1;
}

seq_t a(idx_t n)
{
    init_fib_part();

    seq_t result = 1;
    if (n == 1) return result;

    uint64_t temp = (uint64_t)n;

    /* Factor 2 */
    if (temp % 2 == 0) {
        int exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++exp;
        }
        result *= fib_part[exp];
    }

    /* Factor odd primes */
    uint64_t p = 3;
    while (p <= isqrt_u64(temp)) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= fib_part[exp];
        }
        p += 2;
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        result *= fib_part[1];  /* exponent 1 gives A003107(1) = 1 */
    }

    return result;
}
