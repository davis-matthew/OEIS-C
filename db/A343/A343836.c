/*
 * A343836  Array T(n, k), n, k > 0, read by antidiagonals; the balanced
 *          ternary representation of T(n, k) is obtained by adding
 *          componentwise (i.e., without carries) the digits in the balanced
 *          ternary representations of n and of k.
 * https://oeis.org/A343836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, base
 * Formula:   a(n) is the nth term of array T(n,k) read by antidiagonals,
 *            where T(n,k) is defined by componentwise addition of the
 *            balanced ternary representations of n and k without carries
 * Range:     i8 n <= 8129, i16 n <= 10010, i32 n <= 10010, i64 n <= 10010,
 *            i128 n <= 10010, big any n
 * Validated: standard, pass, 94 terms through n = 9199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Convert a non‑negative integer to its balanced ternary digits.
   The digits are stored low‑to‑high in out[0]..out[len-1].
   Balanced ternary digits are in the set {-1,0,1}. */
static void balanced_digits(idx_t n, int *out, size_t *len)
{
    size_t pos = 0;
    while (n != 0) {
        uint64_t q = n / 3;
        uint64_t r = n % 3;
        if (r == 2) {            /* 2 in base‑3 becomes -1 with a carry */
            out[pos++] = -1;
            n = q + 1;
        } else {                 /* 0 or 1 stays the same */
            out[pos++] = (int)r;
            n = q;
        }
    }
    *len = pos;                  /* zero if n was initially 0 */
}

static inline seq_t balanced_add(idx_t a, idx_t b)
{
    /* maximum needed digits for 64‑bit numbers is < 40, keep room */
    int da[64], db[64];
    size_t na, nb;
    balanced_digits(a, da, &na);
    balanced_digits(b, db, &nb);

    seq_t result = 0;
    seq_t pow3   = 1;
    size_t maxlen = na > nb ? na : nb;

    for (size_t i = 0; i < maxlen; ++i) {
        int va = (i < na) ? da[i] : 0;
        int vb = (i < nb) ? db[i] : 0;
        int sum = va + vb;               /* in range [-2,2] */

        int digit;
        /* Convert the sum to a balanced ternary digit */
        switch (sum) {
            case -2: digit = 1;   break;
            case -1: digit = -1;  break;
            case 0 : digit = 0;   break;
            case 1 : digit = 1;   break;
            case 2 : digit = -1;  break;
            default: digit = 0;   /* unreachable */
        }

        result += (seq_t)digit * pow3;
        pow3 *= 3; /* next power of 3 */
    }

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the diagonal index s such that
       start = s*(s+1)/2 <= n < (s+1)*(s+2)/2          */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    long double root = sqrtl((long double)val);
    idx_t s = (idx_t)((root - 1.0L) / 2.0L);

    /* Adjust for possible rounding errors */
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n) ++s;
    while ((unsigned __int128)s * (s + 1) / 2 > n) --s;

    idx_t start  = s * (s + 1) / 2;
    idx_t offset = n - start;
    idx_t i = s - offset; /* row index in the array */
    idx_t j = offset;     /* column index in the array */

    return balanced_add(i, j);
}
