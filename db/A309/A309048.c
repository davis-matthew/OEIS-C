/*
 * A309048  Expansion of Product_{k>=0} (1 + x^(3^k) + x^(2*3^k) -
 *          x^(3^(k+1))).
 * https://oeis.org/A309048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Recurrence a(0)=a(1)=a(2)=1; a(3*n) = a(n)-a(n-1); a(3*n+1) =
 *            a(3*n+2) = a(n) for n ≥ 0 (with a(-1) unused)
 * Range:     i8 n <= 109, i16 n <= 109, i32 n <= 109, i64 n <= 109,
 *            i128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Cache size: large enough for n up to about 10000 (problem statement) */
#define CACHE_MAX 200000

static seq_t  cache[CACHE_MAX];
static uint8_t seen[CACHE_MAX] = {0};

static seq_t compute_value(idx_t n)
{
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

    if (seen[n])
        return cache[n];

    idx_t q = n / 3;

    seq_t result;
    if (n % 3 == 0) {
        seq_t a_q  = compute_value(q);
        seq_t a_q1 = compute_value(q - 1);
        result = a_q - a_q1;          /* a(3q) = a(q) - a(q-1)   */
    } else {                           /* a(3q+1) = a(3q+2) = a(q) */
        result = compute_value(q);
    }

    cache[n] = result;
    seen[n]  = 1;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)1;
    return compute_value(n);
}
