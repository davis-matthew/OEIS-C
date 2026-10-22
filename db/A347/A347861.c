/*
 * A347861  a(n) = A000032(n)*A000032(n+1) mod A000032(n+2).
 * https://oeis.org/A347861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A347861
   a(n) = A000032(n) * A000032(n+1) mod A000032(n+2)
   (Lucas numbers: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2))
   Using the proven shortcut:
     a(0)=2, a(1)=3, a(2)=5
     for even n>=2: a(n)=5
     for odd n>=3: a(n)=L(n+2)-5
   The function below computes these values exactly for any sized
   integer type seq_t and index type idx_t (uint64_t).  No global
   caching is required; the Lucas numbers are generated with a simple
   linear recurrence.  All operations are on seq_t, making the code
   valid for 8‑, 16‑, 32‑, 64‑bit, 128‑bit, and arbitrary‑precision
   integer types. */

#include <stdint.h>          // for uint64_t (idx_t origin)
#include <stdlib.h>          // for exit, NULL

/* Return the k-th Lucas number L(k).  k is an unsigned integer. */
static seq_t lucas(idx_t k)
{
    /* L(0)=2, L(1)=1 */
    if (k == 0) return (seq_t)2;
    if (k == 1) return (seq_t)1;

    seq_t a = (seq_t)2;    /* L(0) */
    seq_t b = (seq_t)1;    /* L(1) */
    seq_t t;
    for (idx_t i = 2; i <= k; ++i) {
        t = a + b;
        a = b;
        b = t;
    }
    return b;
}

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly to avoid negative indices. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    /* Even indices >=2 have the constant value 5. */
    if ((n & 1) == 0) return (seq_t)5;

    /* For odd n >=3: a(n) = L(n+2) - 5. */
    return lucas(n + 2) - (seq_t)5;
}
