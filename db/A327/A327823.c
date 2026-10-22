/*
 * A327823  Odd integers m such that every odd integer k with 1 < k < m and
 *          gcd(k,m) = 1 is prime.
 * https://oeis.org/A327823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   gcd, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property sequence: a(n) = nth odd integer m such that every odd k with 1<k<m
   and gcd(k,m)=1 is prime. This sequence ends at m = 105. */

/* The sequence is computed by brute‑force search up to the known upper bound 105.
   No auxiliary data structures are required. */
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/isPrimeGeneric.h"

static seq_t _seq[128];
static idx_t _seq_len = 0;

/* Precompute all terms up to the known maximum (105).  */
static void _precompute(void) {
    if (_seq_len != 0) return;          /* already done */
    for (idx_t m = 1; m <= 105; m += 2) {   /* only odd m */
        bool ok = true;
        for (idx_t k = 3; k < m; k += 2) {   /* odd k 1<k<m */
            seq_t g = gcd((seq_t)m, (seq_t)k);
            if (g == 1 && !isPrimeGeneric((seq_t)k)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            _seq[_seq_len++] = (seq_t)m;
        }
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined for n=0 or negative */
    _precompute();
    if (n-1 < _seq_len) {
        return _seq[n-1];
    } else {
        /* sequence has no further terms beyond 105 */
        return 0;
    }
}
