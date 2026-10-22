/*
 * A372938  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals, where
 *          T(n,k) = Sum_{1 <= x_1, x_2, ..., x_k <= n} gcd(x_1, x_2, ...,
 *          x_k, n)^k.
 * https://oeis.org/A372938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A372938
 *  
 *  a(n) is the nth term obtained by reading the square array
 *  T(n,k) along antidiagonals.  For a given n the coordinates
 *  (row, col) are:
 *      d   = smallest integer with d*(d+1)/2 >= n
 *      prev = (d-1)*d/2                     // elements before this diagonal
 *      r   = n - prev                        // rank (1‑based) within the diagonal
 *      row = r
 *      col = d - r + 1
 *  T(n,k) is defined by
 *      T(n,k) = Σ_{d|n} d^k * J_k(n/d)
 *    where J_k is the k‑th Jordan totient.
 *  T(n,k) is multiplicative in n and for a prime power
 *      T(p^e,k) = Σ_{j=0}^{e} p^{k j} * [ (e-j==0)?1 : p^{k(e-j)}-p^{k(e-j-1)} ]
 *
 *  The implementation factorises n by trial division (no external libs),
 *  then evaluates the above product.  All arithmetic uses seq_t; no
 *  intermediate overflows occur if the final result fits in seq_t.
 */

#include <stddef.h> /* for size_t */

/* Compute diagonal number for index n */
static idx_t diagonal_from_index(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Contribution of a single prime power p^e to T(p^e, k) */
static seq_t T_prime(uint64_t p, uint32_t e, idx_t k)
{
    /* Compute p^(k* m) for m = 0 .. e */
    seq_t powpk = 1;
    for (idx_t t = 0; t < k; ++t)
        powpk *= (seq_t)p;

    seq_t powkm[20];      /* e ≤ 13 for n ≤ 10000, so 20 is safe */
    powkm[0] = 1;
    for (uint32_t m = 1; m <= e; ++m)
        powkm[m] = powkm[m - 1] * powpk;

    seq_t sum = 0;
    for (uint32_t j = 0; j <= e; ++j) {
        uint32_t m = e - j;
        seq_t term;
        if (m == 0) {                     /* n/d = 1 */
            term = powkm[j];
        } else {
            term = powkm[j] * (powkm[m] - powkm[m - 1]);
        }
        sum += term;
    }
    return sum;
}

/* Factorise n (≤ 10000) and compute T(n, k) */
static seq_t compute_T(uint64_t n, idx_t k)
{
    if (n <= 1)
        return 1;

    uint64_t tmp = n;
    uint64_t primes_local[10];
    uint32_t exps[10];
    int pcnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes_local[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes_local[pcnt] = tmp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    seq_t result = 1;
    for (int i = 0; i < pcnt; ++i) {
        seq_t part = T_prime(primes_local[i], exps[i], k);
        result *= part;
    }
    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t d = diagonal_from_index(n);          /* diagonal number */
    idx_t prev = ((d - 1) * d) / 2;            /* elements before this diagonal */
    idx_t r = n - prev;                        /* rank inside the diagonal (1‑based) */
    idx_t row = r;                             /* first argument of T          */
    idx_t col = d - r + 1;                     /* second argument of T         */

    return compute_T((uint64_t)row, col);
}
