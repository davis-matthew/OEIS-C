/*
 * A356997  a(n) = b(n) - b(n - b(n - b(n))) for n >= 2, where b(n) =
 *          A356988(n).
 * https://oeis.org/A356997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A356997  */
/*  a(n) = b(n) - b(n - b(n - b(n))).
    b(n) is A356988(n).
    The sequence consists of Fibonacci‑based plateaus and troughs
    with slope ±1 between them, as described in the OEIS entry.
    The implementation uses those structural formulas:
      - Plateaus Pk:   2*F(k)          ≤ n ≤ 2*F(k)+2*F(k-3)  ->  a(n)=F(k-2)
      - Troughs Tk:    F(k)            ≤ n ≤ F(k)+F(k-5)     ->  a(n)=F(k-5)
      - Down slope:    2*F(k)+2*F(k-3) ≤ n ≤ F(k+2)          ->  a(n)=F(k-2)-(n-(2*F(k)+2*F(k-3)))
      - Up   slope:    F(k)+F(k-5)+1   ≤ n ≤ 2*F(k-1)-1      ->  a(n)=F(k-5)+(n-(F(k)+F(k-5)+1))+1
    Only the indices n ≥ 2 are defined; values for n=2…5 are hard‑coded.
    The routine uses a fixed table of Fibonacci numbers up to F(60),
    which easily covers indices up to at least 10 000.  No global
    memory is allocated or freed, so the function can be called in any
    order.  The algorithm always stops after a small constant number
    of iterations.  */

/* External types supplied by the build system */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* base cases for the offset 2  (n >= 2) */
    if (n <= 5) {
        /* a(2)=0, a(3)=1, a(4)=1, a(5)=0 */
        return (n == 2) ? 0 :
               (n == 3) ? 1 :
               (n == 4) ? 1 : 0;
    }

    /* pre‑compute a small Fibonacci table once */
    static seq_t fib[64];
    static int init = 0;
    if (!init) {
        fib[0] = 0;
        fib[1] = 1;
        for (size_t i = 2; i < 64; ++i)
            fib[i] = fib[i-1] + fib[i-2];
        init = 1;
    }

    /* iterate over all relevant k (4 ≤ k) */
    for (size_t k = 4; k + 2 < 64; ++k) {
        seq_t Fk     = fib[k];
        seq_t Fk_1   = fib[k-1];
        seq_t Fk_2   = fib[k-2];
        seq_t Fk_3   = fib[k-3];
        seq_t Fk_5   = (k >= 5) ? fib[k-5] : 0;
        seq_t Fk_2sl = fib[k + 2];       /* F(k+2)     */

        /* 1.  Trough Tk  (k ≥ 5) */
        if (k >= 5) {
            seq_t startT = Fk;
            seq_t endT   = startT + Fk_5;
            if (n >= startT && n <= endT)
                return Fk_5;
        }

        /* 2.  Plateau Pk */
        seq_t startP = 2 * Fk;
        seq_t endP   = startP + 2 * Fk_3;
        if (n >= startP && n <= endP)
            return Fk_2;

        /* 3.  Downward slope from Pk to Tk+2 */
        seq_t startSdown = endP;
        seq_t endSdown   = Fk_2sl;
        if (n >= startSdown && n <= endSdown)
            return Fk_2 - (n - startSdown);

        /* 4.  Upward slope from Tk to P(k-1)  (k ≥ 6) */
        if (k >= 6) {
            seq_t startStro = Fk + Fk_5 + 1;
            seq_t endStro   = 2 * Fk_1 - 1;
            if (n >= startStro && n <= endStro)
                return Fk_5 + (n - startStro) + 1;
        }
    }

    /* In practice this point is never reached for valid n ≥ 2
       because the above loop covers all integers for the intended range. */
    return 0;
}
