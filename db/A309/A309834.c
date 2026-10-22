/*
 * A309834  Sum of the even parts appearing among the smallest parts of the
 *          partitions of n into 5 parts.
 * https://oeis.org/A309834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{l=1..⌊n/5⌋} Sum_{k=l..⌊(n-l)/4⌋}
 *            Sum_{j=k..⌊(n-k-l)/3⌋} Sum_{i=j..⌊(n-j-k-l)/2⌋} l * ((l-1) mod
 *            2) Recurrence (valid for n > 29): a(n) =
 *            a(n-1)+a(n-2)-2*a(n-5)+a(n-8)+a(n-9)+a(n-10)-2*a(n-11)-2*a(n-12)
 *            +4*a(n-15)-2*a(n-18)-2*a(n-19)+a(n-20)+a(n-21)+a(n-22)
 *            -2*a(n-25)+a(n-28)+a(n-29)-a...
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   brute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values (n = 0..29) are computed via brute force. */

#include <stdlib.h>
#include "../utils/brute.h"

seq_t a(idx_t n)
{
    seq_t *arr = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!arr) return 0;   /* memory allocation failed */

    for (idx_t i = 0; i <= n; ++i) {
        if (i < 30) {
            arr[i] = brute(i);
        } else {
            seq_t sumPos = 0;
            sumPos += arr[i - 1];
            sumPos += arr[i - 2];
            sumPos += arr[i - 8];
            sumPos += arr[i - 9];
            sumPos += arr[i - 10];
            /* 4 * arr[i-15] */
            seq_t t = arr[i - 15];
            sumPos += t + t + t + t;
            sumPos += arr[i - 20];
            sumPos += arr[i - 21];
            sumPos += arr[i - 22];
            sumPos += arr[i - 28];
            sumPos += arr[i - 29];

            seq_t sumNeg = 0;
            t = arr[i - 5];
            sumNeg += t + t;
            t = arr[i - 11];
            sumNeg += t + t;
            t = arr[i - 12];
            sumNeg += t + t;
            t = arr[i - 18];
            sumNeg += t + t;
            t = arr[i - 19];
            sumNeg += t + t;
            t = arr[i - 25];
            sumNeg += t + t;
            sumNeg += arr[i - 30];   /* coefficient 1 */

            arr[i] = sumPos - sumNeg;
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
