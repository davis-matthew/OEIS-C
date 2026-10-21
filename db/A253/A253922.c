/*
 * A253922  Indices of centered pentagonal numbers (A005891) which are also
 *          octagonal numbers (A000567).
 * https://oeis.org/A253922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+482*a(n-2)-482*a(n-3)-a(n-4)+a(n-5); Initial
 *            values: a(1)=1, a(2)=56, a(3)=297, a(4)=26752, a(5)=142913
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is very fast growing; values up to n≈10 000
       comfortably fit in an array of that size.  We store up to the
       largest index requested so far, computing new terms only when
       necessary. */
    static seq_t cache[10001];
    static idx_t upto = 0;

    /* Establish the first five terms – always available for the
       recurrence to work for future indices.  We do this once only. */
    if (upto == 0)
    {
        cache[1] = 1;
        cache[2] = 56;
        cache[3] = 297;
        cache[4] = 26752;
        cache[5] = 142913;
        upto = 5;
    }

    /* If a larger index is requested, compute successive terms
       using the linear recurrence until the desired one is reached. */
    if (n > upto)
    {
        for (idx_t i = upto + 1; i <= n; ++i)
        {
            cache[i] = cache[i-1] + (seq_t)482 * cache[i-2]
                           - (seq_t)482 * cache[i-3]
                           - cache[i-4] + cache[i-5];
        }
        upto = n;
    }

    /* For n == 0 the offset is 1, but the function is only called
       with n ≥ 1 in the test harness.  Returning 0 for safety. */
    return (n == 0) ? 0 : cache[n];
}
