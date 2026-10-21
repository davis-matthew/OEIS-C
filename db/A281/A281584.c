/*
 * A281584  Solutions x to the negative Pell equation x^2 - 15*y^2 = -11 with
 *          x, y > 0.
 * https://oeis.org/A281584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 8*a(n-2) - a(n-4) for n > 4
   initial values: a(1)=2, a(2)=7, a(3)=23, a(4)=58
   The function works for any unsigned seq_t type used in the build.
   It does not require any external utilities and runs in O(n) time,
   using only a small fixed buffer. */

seq_t a(idx_t n)
{
    /* handle first four terms explicitly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)23;
    if (n == 4) return (seq_t)58;

    /* arr[0]=a(n-4), arr[1]=a(n-3), arr[2]=a(n-2), arr[3]=a(n-1) */
    seq_t arr[4];
    arr[0] = (seq_t)2;
    arr[1] = (seq_t)7;
    arr[2] = (seq_t)23;
    arr[3] = (seq_t)58;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)8 * arr[2] - arr[0];  /* a(i) = 8*a(i-2) - a(i-4) */
        /* shift array left */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = next;
    }
    return arr[3];  /* a(n) */
}
