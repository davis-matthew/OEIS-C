/*
 * A308551  Start with an empty stack S; for n = 1, 2, 3, ..., interpret the
 *          binary representation of n from left to right as follows: in case
 *          of bit 1, push the number 1 on top of S, in case of bit 0, replace
 *          the two numbers on top of S with their sum; a(n) gives the number
 *          on top of S after processing n.
 * https://oeis.org/A308551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   Simulate a persistent stack according to the definition of
 *            A308551 where for each n the binary representation of n is
 *            processed from left to right: bit 1 pushes a 1, bit 0 replaces
 *            the two top elements with their sum. The top element after
 *            processing n is returned as a(n). The stack and previousl...
 * Range:     u8 n <= 295, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *stack = NULL;        /* dynamic stack of values */
static size_t stack_cap = 0;       /* capacity of stack */
static size_t stack_size = 0;      /* current number of items on stack */

static seq_t *seq_vals = NULL;     /* stored a(n) values, 1‑based index */
static size_t seq_cap = 0;         /* capacity of seq_vals */
static idx_t seq_len = 0;          /* number of values already computed */

/* Push a value onto the stack, growing it if necessary. */
static void push_stack(seq_t val) {
    if (stack_size == stack_cap) {
        size_t new_cap = stack_cap ? stack_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(stack, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        stack = tmp;
        stack_cap = new_cap;
    }
    stack[stack_size++] = val;
}

/* Replace the two top values on the stack with their sum, growing the
   stack if it was full.  The call is only made when stack_size >= 2. */
static void replace_top_two_with_sum(void) {
    seq_t b = stack[--stack_size];
    seq_t a_val = stack[--stack_size];
    seq_t sum = a_val + b;
    push_stack(sum);
}

/* Compute a(n) for all n up to the requested value, using the cached
   state.  The function is thread‑unsafe but satisfies the specification. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* defined only for n >= 1 */

    /* Generate the sequence up to n if necessary */
    if (n > seq_len) {
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            /* Find the position of the most significant 1 bit */
            idx_t tmp = i;
            int msb = 0;
            while (tmp >>= 1) ++msb;

            /* Process bits from most significant to least */
            for (int pos = msb; pos >= 0; --pos) {
                int bit = (int)((i >> pos) & 1ULL);
                if (bit) {          /* push 1 */
                    push_stack((seq_t)1);
                } else {            /* replace two tops by sum */
                    replace_top_two_with_sum();
                }
            }

            /* Store the top of the stack as a(i) */
            seq_t top = stack[stack_size - 1];
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq_vals = tmp;
                seq_cap = new_cap;
            }
            seq_vals[seq_len++] = top;
        }
    }

    /* Return the pre‑computed value */
    return seq_vals[n - 1];
}
