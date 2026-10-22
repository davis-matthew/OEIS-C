/*
 * A376202  Number of pairs 1 <= x <= y <= n-1 such that gcd(x,n) = gcd(y,n) =
 *          gcd(x+y,n) = 1 and 1/x + 1/y == 1/(x+y) mod n.
 * https://oeis.org/A376202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs 1 ≤ x ≤ y ≤ n‑1 such that gcd(x,n) =
 *            gcd(y,n) = gcd(x+y,n) = 1 and inv(x) + inv(y) ≡ inv(x+y) (mod
 *            n), where inv(a) denotes the modular inverse of a modulo n
 * Range:     u8 n <= 216, u16 n <= 6000, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include needed headers */
#include <stdlib.h>
#include <stdint.h>

/* Helper: Euclidean GCD for idx_t (unsigned). */
static inline idx_t ugcd(idx_t a, idx_t b)
{
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

/* Helper: modular inverse of a modulo m, assumes gcd(a,m)=1.
   Returns 0 if no inverse exists (should not happen for units). */
static inline idx_t modinv(idx_t a, idx_t m)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)(a % m);
    while (newr != 0) {
        idx_t q = (idx_t)(r / newr);
        int64_t tmp = t - (int64_t)q * newt; t = newt; newt = tmp;
        tmp = r - (int64_t)q * newr; r = newr; newr = tmp;
    }
    if (r > 1) return 0;          /* no inverse, but caller ensures gcd=1 */
    if (t < 0) t += m;
    return (idx_t)t;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    /* Pre‑allocate array to store modular inverses for units.
       0 ⇒ not a unit; else contains inverse modulo n. */
    uint64_t *inv_mod = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!inv_mod) return 0;
    for (idx_t i = 0; i < n; ++i) inv_mod[i] = 0;

    /* Gather units of modulo n into an array */
    idx_t *units = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!units) { free(inv_mod); return 0; }
    size_t uc = 0;
    for (idx_t i = 1; i < n; ++i) {
        if (ugcd(i, n) == 1) {
            units[uc++] = i;
            inv_mod[i] = modinv(i, n);
        }
    }

    /* Count valid pairs */
    seq_t count = 0;
    for (size_t aidx = 0; aidx < uc; ++aidx) {
        idx_t x  = units[aidx];
        uint64_t invx = inv_mod[x];
        for (size_t bidx = aidx; bidx < uc; ++bidx) {
            idx_t y  = units[bidx];
            uint64_t invy = inv_mod[y];
            idx_t s = (x + y) % n;
            uint64_t invs = inv_mod[s];
            if (invs == 0) continue;            /* s not coprime to n */
            if ((invx + invy) % n == invs)
                ++count;
        }
    }

    free(units);
    free(inv_mod);
    return count;
}
