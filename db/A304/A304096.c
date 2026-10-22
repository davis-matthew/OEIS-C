/*
 * A304096  Number of Lucas numbers larger than 3 (4, 7, 11, 18, ...) that
 *          divide n.
 * https://oeis.org/A304096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are Lucas numbers > 3 (Lucas
 *            numbers: 2,1,3,4,7,11,18,29,...). Count only divisors d > 3
 * Range:     u8 n <= 5477, u16 n <= 9756, u32 n <= 9756, u64 n <= 9756,
 *            u128 n <= 9756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

static seq_t lucas_arr[200];
static size_t lucas_len = 0;
static idx_t lucas_max = 0;

/* Precompute all Lucas numbers ≤ limit into lucas_arr.  */
static void ensure_lucas(idx_t limit)
{
    if (limit <= lucas_max && lucas_len > 0)
        return;   /* already have enough entries */

    seq_t a = (seq_t)2;   /* L0 */
    seq_t b = (seq_t)1;   /* L1 */
    size_t i = 0;

    while (a <= limit) {
        if (i >= sizeof(lucas_arr)/sizeof(lucas_arr[0]))
            break;      /* safety: avoid overrunning the static array */

        lucas_arr[i++] = a;
        seq_t c = a + b;  /* next Lucas number */
        a = b;
        b = c;
    }
    lucas_len = i;
    lucas_max = limit;
}

/* Binary search in lucas_arr for value v.  Returns 1 if found.  */
static int is_lucas_val(seq_t v)
{
    size_t lo = 0, hi = lucas_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        seq_t mv = lucas_arr[mid];
        if (mv == v)
            return 1;
        if (mv < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n < 4)     /* no Lucas number > 3 divides any integer < 4 */
        return (seq_t)0;

    /* Prepare Lucas numbers up to n.  */
    ensure_lucas(n);

    seq_t count = 0;

    /* Enumerate divisors efficiently: avoid i*i overflow by using i <= n/i.  */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 > 3 && is_lucas_val((seq_t)d1))
                ++count;

            if (d2 != d1 && d2 > 3 && is_lucas_val((seq_t)d2))
                ++count;
        }
    }

    return count;
}
