/*
 * A241504  a(n) = |{0 < g < prime(n): g is not only a primitive root modulo
 *          prime(n) but also a partition number given by A000041}|.
 * https://oeis.org/A241504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers g with 0 < g < prime(n) such that * g
 *            is a primitive root mod prime(n) and g is a partition number
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_phi, get_prime, pow_mod_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/factor_phi.h"
#include "../utils/pow_mod_uint.h"

/* Pre‑computed partition numbers up to a value > 200000 (sufficient for primes <= 1e5) */
static seq_t part_vals[200];
static size_t part_cnt = 0;
static bool part_init = false;

static void init_partitions(void)
{
    if (part_init) return;
    part_init = true;

    const int MAXN = 200;        /* enough for values up to ~1e5 */
    int64_t ptmp[MAXN + 1];
    ptmp[0] = 1;
    for (int n = 1; n <= MAXN; ++n) {
        int64_t sum = 0;
        for (int k = 1; ; ++k) {
            int pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            int sign = (k & 1) ? 1 : -1;
            sum += sign * ptmp[n - pent1];
            int pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= n)
                sum += sign * ptmp[n - pent2];
        }
        ptmp[n] = sum;
    }

    /* store until value exceeds 200000 (enough for all needed g) */
    for (int i = 0; i <= MAXN; ++i) {
        if ((seq_t)ptmp[i] > 200000ULL) {
            part_cnt = i;
            break;
        }
        part_vals[i] = (seq_t)ptmp[i];
    }
}

/* Check if a given unsigned integer is a partition number */
static bool is_partition(uint64_t g)
{
    init_partitions();
    for (size_t i = 0; i < part_cnt; ++i) {
        seq_t val = part_vals[i];
        if (val == (seq_t)g)
            return true;
        if (val > (seq_t)g)
            break;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but safe */
    uint64_t p = get_prime((idx_t)n);     /* prime(n), 1‑indexed */
    uint64_t phi = p - 1;
    uint64_t factors[64];                 /* enough for phi <= 1e5 */
    size_t fac_cnt;
    factor_phi(phi, factors, &fac_cnt);

    seq_t count = 0;
    for (uint64_t g = 1; g < p; ++g) {
        if (!is_partition(g))
            continue;

        bool ok = true;
        for (size_t i = 0; i < fac_cnt; ++i) {
            uint64_t q = factors[i];
            if (pow_mod_uint(g, phi / q, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok)
            ++count;
    }
    return count;
}
