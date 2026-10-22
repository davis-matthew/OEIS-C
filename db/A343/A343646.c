/*
 * A343646  a(0) = 0, and for any n > 0 such that F(k) <= n < F(k+1) for some
 *          k > 1, a(n) = F(k) + a(F(k+1) - n - 1) (where F(k) = A000045(k) is
 *          the k-th Fibonacci number).
 * https://oeis.org/A343646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; * For n>0, let k>1 such that F(k) ≤ n < F(k+1) (F =
 *            Fibonacci). * Then a(n)=F(k)+a(F(k+1)-n-1)
 * Range:     u8 n <= 232, u16 n <= 8305, u32 n <= 8305, u64 n <= 8305,
 *            u128 n <= 8305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t fib_u64[95];
static unsigned __int128 fib128[95];

/* Initialise Fibonacci tables once */
static void init_fibs(void) {
    static bool done = false;
    if (done) return;
    fib_u64[0] = 0;
    fib_u64[1] = 1;
    for (int i = 2; i <= 93; ++i)
        fib_u64[i] = fib_u64[i-1] + fib_u64[i-2];

    fib128[0] = 0;
    fib128[1] = 1;
    for (int i = 2; i <= 94; ++i)
        fib128[i] = fib128[i-1] + fib128[i-2];
    done = true;
}

/* Recursive computation of A343646 */
seq_t a(idx_t n) {
    init_fibs();
    if (n == 0) return 0;

    /* Find k such that F(k) ≤ n < F(k+1), k ≥ 2 */
    int k = 2;
    /* loop at most 93 times – very cheap */
    while (k + 1 <= 93 && fib_u64[k+1] <= n)
        ++k;

    /* Compute m = F(k+1) - n - 1 */
    unsigned __int128 fk1 = fib128[k+1];
    unsigned __int128 m128 = fk1 - (unsigned __int128)n - 1;
    idx_t m = (idx_t)m128;   /* m fits into idx_t because it is < n */

    return (seq_t)fib_u64[k] + a(m);
}
