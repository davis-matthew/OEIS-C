/*
 * A257948  Length of cycle in which n ends under iteration of
 *          sum-of-squares-of-two-digits map s_2.
 * https://oeis.org/A257948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the cycle reached by iterating * the
 *            sum‑of‑squares‑of‑two‑digits map s_2 starting from n. * s_2(n)
 *            is defined by repeatedly taking the decimal digits of n * in
 *            groups of two (from the least significant side) and summing *
 *            the squares of these groups. For a single remaining digit...
 * Range:     u8 n <= 1, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Forward declaration of the utility function for a full
 * implementation.  It is not needed here, but keeping the
 * includes minimal ensures C11 compliance. */
static seq_t s2(seq_t n);

/* Helper: compute s_2(n) for a given number n.
 * Handles numbers of any length; groups digits from the
 * least significant side, which produces the correct sum
 * because addition is commutative. */
static seq_t s2(seq_t n) {
    seq_t sum = 0;
    while (n > (seq_t)0) {
        seq_t pair = n % (seq_t)100;
        sum += pair * pair;
        n /= (seq_t)100;
    }
    return sum;
}

/* Main sequence function.  External linkage is required. */
seq_t a(idx_t n) {
    seq_t start = (seq_t)n;

    /* Phase 1: find a meeting point of tortoise and hare. */
    seq_t tortoise = s2(start);
    seq_t hare = s2(s2(start));
    while (tortoise != hare) {
        tortoise = s2(tortoise);
        hare = s2(s2(hare));
    }

    /* Phase 2: find the start of the cycle. */
    seq_t ptr1 = start;
    seq_t ptr2 = hare;
    while (ptr1 != ptr2) {
        ptr1 = s2(ptr1);
        ptr2 = s2(ptr2);
    }
    seq_t cycle_start = ptr1;

    /* Phase 3: compute the length of the cycle. */
    seq_t length = 1;
    seq_t current = s2(cycle_start);
    while (current != cycle_start) {
        current = s2(current);
        ++length;
    }
    return length;
}
