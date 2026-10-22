/*
 * A399926  Irregular triangle read by rows: T(n,k) = Product_{primes p
 *          divides n} (p - [m == 1 (mod p)]), where m = A038566(n,k) is the
 *          k-th number coprime to n, and [] is an Iverson bracket.
 * https://oeis.org/A399926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = flatten by rows of the irregular triangle T(n,k) =
 *            Π_{p|n} (p - [m ≡ 1 (mod p)]) where m = the k‑th integer coprime
 *            to n (listed in increasing order)
 * Range:     u8 n <= 9995, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- Required headers ---------- */
#include <stdint.h>
#include <stddef.h>

/* ---------- Helper: gcd for idx_t ---------- */
static idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- Helper: Euler's totient φ(n) (returns idx_t, fits in 64‑bit) ---------- */
static idx_t _phi_u64(idx_t n)
{
    if (n <= 1) return (idx_t)1;
    idx_t result = n;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result = (result / p) * (p - 1);
        }
    }
    if (t > 1) {
        result = (result / t) * (t - 1);
    }
    return result;
}

/* ---------- Helper: compute the k‑th coprime to n (1‑based) ---------- */
static idx_t _kth_coprime(idx_t n, idx_t k)
{
    idx_t count = 0;
    for (idx_t x = 1; x <= n; ++x) {
        if (_gcd_idx(x, n) == 1) {
            ++count;
            if (count == k) return x;
        }
    }
    return (idx_t)1; /* never reached for valid k */
}

/* ---------- Helper: T(row, k) as defined above ---------- */
static seq_t _T_of(idx_t row, idx_t k)
{
    /* find the k‑th coprime m */
    idx_t m = _kth_coprime(row, k);

    /* compute product over prime divisors of row */
    seq_t prod = (seq_t)1;
    idx_t t = row;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            /* multiply appropriate factor */
            seq_t factor = (m % p == 1) ? (seq_t)(p - 1) : (seq_t)p;
            prod *= factor;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) {          /* remaining prime factor */
        seq_t factor = (m % t == 1) ? (seq_t)(t - 1) : (seq_t)t;
        prod *= factor;
    }
    return prod;
}

/* ---------- Public entry point ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1, but keep defined */

    idx_t cumulative = 0;
    idx_t row = 1;

    while (1) {
        idx_t phi_row = _phi_u64(row);
        if (cumulative + phi_row >= n) break;
        cumulative += phi_row;
        ++row;
    }

    /* position in current row (1‑based) */
    idx_t k = n - cumulative;
    return _T_of(row, k);
}
