/*
 * A370887  Square array read by descending antidiagonals: T(n,k) is the
 *          number of subgroups of the elementary abelian group of order
 *          A000040(k)^n for n >= 0 and k >= 1.
 * https://oeis.org/A370887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 40, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   _T_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A370887
 *  T(n,k) = 2*T(n-1,k) + (p^(n-1)-1)*T(n-2,k)
 *  with T(0,k)=1, T(1,k)=2,   p = k‑th prime.
 *  The sequence a(n) is obtained by reading the array T(n,k)
 *  by descending antidiagonals:  for a given diagonal s
 *  (where s = n + k - 1) the terms are taken with n increasing.
 *  a(n) is the element at sequence index n.  No other output or
 *  input is performed; the function works for all n >= 0 and
 *  calls in any order.  It is fully self‑contained and uses
 *  only integer arithmetic. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/_T_of.h"

/* --- Prime table ------------------------------------------------------- */
/* Store the first 2048 primes; they easily cover all indices needed
 * for n up to several million (s ≈ sqrt(2n)).  They are generated
 * once on first use by a simple trial‑division sieve. */
static uint64_t _prime_table[2048];
static size_t _prime_count = 0;

static void _ensure_primes(void)
{
    if (_prime_count > 0)
        return;                    /* already generated */

    _prime_count = 0;
    for (uint64_t cand = 2; _prime_count < 2048; ++cand) {
        bool prime = true;
        for (size_t i = 0; i < _prime_count; ++i) {
            uint64_t p = _prime_table[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            _prime_table[_prime_count++] = cand;
        }
    }
}

static seq_t _get_kth_prime(idx_t k)
{
    /* k is 1‑based.  This function is safe for k <= 2048. */
    _ensure_primes();
    return (seq_t)_prime_table[k - 1];
}

/* --- Gaussian‑binomial sum ------------------------------------------- */

/* --- Main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the diagonal 's' such that the number of terms before it
     * is s*(s+1)/2 and s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t prev_terms = s * (s + 1) / 2;   /* terms before this diagonal */
    idx_t offset     = n - prev_terms;   /* 0‑based within diagonal */

    idx_t row = offset;                 /* n in T(n,k)   */
    idx_t col = s - offset + 1;          /* k in T(n,k) (1‑based) */

    seq_t prime = _get_kth_prime(col);

    return _T_of(row, prime);
}
