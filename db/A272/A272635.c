/*
 * A272635  Numbers that are not a sum or a difference of two Fibonacci
 *          numbers.
 * https://oeis.org/A272635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer that cannot be written as the sum
 *            or difference of two Fibonacci numbers (with F(1)=1, F(2)=1)
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system.
   We rely on seq_t supporting the usual arithmetic and comparison operators.
*/

/* --- Fibonacci cache --------------------------------------------------- */
static seq_t *fib = NULL;         /* array of Fibonacci numbers */
static size_t fib_size = 0;       /* number of Fibonacci numbers stored */
static size_t fib_cap  = 0;       /* capacity of the array */

/* Ensure that fib contains all Fibonacci numbers up to strictly greater than
   'limit'.  Fib numbers are 1,1,2,3,5,...  */
static void ensure_fib_limit(seq_t limit)
{
    /* Allocate initially */
    if (fib_size == 0) {
        fib_cap = 4;
        fib = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        if (!fib) return;               /* allocation failed – ignore here */
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)1;
        fib_size = 2;
    }

    /* Grow until fib[fib_size-1] > limit */
    while (fib[fib_size - 1] <= limit) {
        if (fib_size == fib_cap) {
            fib_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(fib, fib_cap * sizeof(seq_t));
            if (!tmp) return;           /* allocation failed – ignore */
            fib = tmp;
        }
        seq_t next = fib[fib_size - 1] + fib[fib_size - 2];
        fib[fib_size] = next;
        ++fib_size;
    }
}

/* Binary search: return true if val is a Fibonacci number. */
static bool is_fib(seq_t val)
{
    size_t lo = 0, hi = fib_size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (fib[mid] == val)
            return true;
        if (fib[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Test whether v can be expressed as sum or difference of two Fibonacci numbers. */
static bool sum_or_diff(seq_t v)
{
    /* Sum check: f1 + f2 = v */
    for (size_t i = 0; i < fib_size && fib[i] <= v; ++i) {
        seq_t target = v - fib[i];
        if (is_fib(target))
            return true;
    }

    /* Difference check: f2 - f1 = v  =>  f1 = f2 - v */
    for (size_t i = 0; i < fib_size; ++i) {
        seq_t f2 = fib[i];
        if (f2 <= v) continue;           /* f2 must be greater than v */
        seq_t f1 = f2 - v;
        if (is_fib(f1))
            return true;
    }

    return false;
}

/* --------------------------------------------------------------------- */
/* Public function required by the test harness.                         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, just return 0 */

    /* Current integer being inspected (as unsigned integer). */
    idx_t i = 0;
    idx_t count = 0;

    while (1) {
        ++i;                                     /* start at 1 */
        ensure_fib_limit((seq_t)i + (seq_t)1);    /* need fib up to > i */

        seq_t val = (seq_t)i;
        if (!sum_or_diff(val)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
