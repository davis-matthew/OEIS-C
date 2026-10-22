/*
 * A333386  Number of solutions to x^y == 0 (mod n) where 0 <= x < n and 1 <=
 *          y <= n.
 * https://oeis.org/A333386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y) with 0 ≤ x < n, 1 ≤ y ≤ n, such
 *            that x^y ≡ 0 (mod n). This implementation follows the
 *            prime‑factorisation description: x must be a multiple of every
 *            prime dividing n. For each such x we compute the minimal y with
 *            y · v_p(x) ≥ k_p for all prime powers p^k_p || n, and a...
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle the trivial offset case */
    if (n == 0) return 0;
    /* Factorise n */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t pcnt = 0;
    uint64_t d = 2;
    while (d * d <= nn) {
        if (nn % d == 0) {
            int cnt = 0;
            while (nn % d == 0) { nn /= d; cnt++; }
            primes[pcnt] = d;
            exps[pcnt]   = cnt;
            pcnt++;
        }
        d += (d == 2 ? 1 : 2);   /* skip even numbers after 2 */
    }
    if (nn > 1) {
        primes[pcnt] = nn;
        exps[pcnt]   = 1;
        pcnt++;
    }

    /* Compute rad(n) and m = n / rad(n) */
    seq_t rad = 1;
    for (size_t i = 0; i < pcnt; i++) rad *= (seq_t)primes[i];
    seq_t m = (seq_t)n / rad;

    /* Count solutions: x = 0 gives n solutions */
    seq_t result = (seq_t)n;

    /* Enumerate x = rad * t  with 1 ≤ t < m */
    seq_t mt = (seq_t)m;
    for (seq_t t = 1; t < mt; ++t) {
        seq_t y_min = 1;
        uint64_t t_val = (uint64_t)t;   /* copy for factoring */
        for (size_t i = 0; i < pcnt; i++) {
            uint64_t p = primes[i];
            int e = 0;
            while (t_val % p == 0) { t_val /= p; e++; }
            seq_t texp = (seq_t)(1 + e);   /* exponent of p in x */
            seq_t cc = (seq_t)(exps[i] + texp - 1) / texp;   /* ceil(k/texp) */
            if (cc > y_min) y_min = cc;
        }
        if (y_min <= (seq_t)n) {
            result += (seq_t)n - y_min + 1;
        }
    }
    return result;
}
