/*
 * A286237  Triangular table: T(n,k) = 0 if k does not divide n, otherwise
 *          T(n,k) = P(phi(k), n/k), where P is sequence A000027 used as a
 *          pairing function N x N -> N, and phi is Euler totient function,
 *          A000010. Table is read by rows as T(1,1), T(2,1), T(2,2), etc.
 * https://oeis.org/A286237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if k ∤ r; otherwise * a(n) = ((φ(k)+ r/k – 2) * (φ(k)+
 *            r/k – 1))/2 + φ(k), * where r and k are the row and column
 *            indices obtained by * triangular indexing of n (row r has
 *            k=1..r)
 * Range:     u8 n <= 276, u16 n <= 9392, u32 n <= 9392, u64 n <= 9392,
 *            u128 n <= 9392, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euler's totient function for 64‑bit integers. */
static seq_t phi_uint64(uint64_t m) {
    if (m <= 1) return 1;
    uint64_t result = m;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result -= result / p;
        }
    }
    if (t > 1) result -= result / t;
    return (seq_t)result;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 ≥ n and (r-1)r/2 < n. */
    idx_t r = 1;
    while ((idx_t)((__int128)r * (r + 1) / 2) < n) ++r;

    /* Column k within the row. */
    idx_t prev_total = (idx_t)((__int128)(r - 1) * r / 2);
    idx_t k = n - prev_total;                 /* 1‑based column index */

    /* If k does not divide r, the entry is zero. */
    if (r % k != 0) return 0;

    seq_t q = (seq_t)(r / k);                 /* quotient n/k */
    seq_t phi_k = phi_uint64((uint64_t)k);    /* φ(k) */

    /* Apply the Cantor pairing formula: ((φ + q – 2) * (φ + q – 1)) / 2 + φ */
    seq_t sum = phi_k + q - 2;
    seq_t prod = sum * (sum + 1) / 2;
    return prod + phi_k;
}
