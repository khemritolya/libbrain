#ifndef LIBBRAIN_LIBBRAIN_H
#define LIBBRAIN_LIBBRAIN_H

#define LIBBRAIN_BUILD_WITH_DEBUG
#undef LIBBRAIN_BUILD_WITH_DEBUG

namespace libbrain {
    enum bfop {
        PLUS,
        MINUS,
        DATA_RIGHT,
        DATA_LEFT,
        OUT,
        IN,
        NOP,
        LOOP_OPEN,
        LOOP_CLOSE,
        END
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops> struct op_at {
        static_assert(sizeof...(ops) + 1 > ins_ptr);
        static inline constexpr bfop operation = op_at<ins_ptr - 1, ops...>::operation;
    };

    template <bfop op, bfop... ops> struct op_at<0, op, ops...> {
        static inline constexpr bfop operation = op;
    };


    template <unsigned int ptr, unsigned int balance, bfop current, bfop op, bfop... ops>
    struct scan_forward {
        static_assert(sizeof...(ops) + 1 > ptr);
        static inline constexpr unsigned int new_ptr =
                scan_forward<ptr + 1, balance, op_at<ptr + 1, op, ops...>::operation, op, ops...>::new_ptr;

    };

    template <unsigned int ptr, unsigned balance, bfop op, bfop... ops>
    struct scan_forward<ptr, balance, LOOP_CLOSE, op, ops...> {
        static inline constexpr unsigned int new_ptr =
                scan_forward<ptr + 1, balance - 1, op_at<ptr + 1, op, ops...>::operation, op, ops...>::new_ptr;
    };

    template <unsigned int ptr, unsigned balance, bfop op, bfop... ops>
    struct scan_forward<ptr, balance, LOOP_OPEN, op, ops...> {
        static inline constexpr unsigned int new_ptr =
                scan_forward<ptr + 1, balance + 1, op_at<ptr + 1, op, ops...>::operation, op, ops...>::new_ptr;
    };

    template <unsigned int ptr, bfop current, bfop op, bfop... ops>
    struct scan_forward<ptr, 0, current, op, ops...> {
        static inline constexpr unsigned int new_ptr = ptr;
    };

    template <unsigned int ptr, bfop op, bfop... ops>
    struct scan_forward<ptr, 0, LOOP_CLOSE, op, ops...> {
        static inline constexpr unsigned int new_ptr = ptr;
    };

    template <unsigned int ptr, bfop op, bfop... ops>
    struct scan_forward<ptr, 0, LOOP_OPEN, op, ops...> {
        static inline constexpr unsigned int new_ptr = ptr;
    };


    template <unsigned int ptr, unsigned int balance, bfop current, bfop op, bfop... ops>
    struct scan_backward {
        static_assert(sizeof...(ops) + 1 > ptr);
        static inline constexpr unsigned int new_ptr =
                scan_backward<ptr - 1, balance, op_at<ptr - 1, op, ops...>::operation, op, ops...>::new_ptr;
    };

    template <unsigned int ptr, unsigned int balance, bfop op, bfop... ops>
    struct scan_backward<ptr, balance, LOOP_OPEN, op, ops...> {
        static inline constexpr unsigned int new_ptr =
                scan_backward<ptr - 1, balance - 1, op_at<ptr - 1, op, ops...>::operation, op, ops...>::new_ptr;
    };

    template <unsigned int ptr, unsigned int balance, bfop op, bfop... ops>
    struct scan_backward<ptr, balance, LOOP_CLOSE, op, ops...> {
        static inline constexpr unsigned int new_ptr =
                scan_backward<ptr - 1, balance + 1, op_at<ptr - 1, op, ops...>::operation, op, ops...>::new_ptr;
    };

    template <unsigned int ptr, bfop current, bfop op, bfop... ops>
    struct scan_backward<ptr, 0, current, op, ops...> {
        static inline constexpr unsigned int new_ptr = ptr;
    };

    template <unsigned int ptr, bfop op, bfop... ops>
    struct scan_backward<ptr, 0, LOOP_OPEN, op, ops...> {
        static inline constexpr unsigned int new_ptr = ptr;
    };

    template <unsigned int ptr, bfop op, bfop... ops>
    struct scan_backward<ptr, 0, LOOP_CLOSE, op, ops...> {
        static inline constexpr unsigned int new_ptr = ptr;
    };


    template <unsigned int ins_ptr, bfop current, bfop op, bfop... ops>
    struct state {
        static_assert(sizeof...(ops) + 1 > ins_ptr);
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            return 2;
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, END, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "END instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            return 0;
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, NOP, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "NOP instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            return state<ins_ptr + 1, op_at<ins_ptr + 1, op, ops...>::operation, op, ops...>
                    ::exec(data, data_ptr, output, input);
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, PLUS, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "PLUS instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            data[data_ptr]++;
            return state<ins_ptr + 1, op_at<ins_ptr + 1, op, ops...>::operation, op, ops...>
                    ::exec(data, data_ptr, output, input);
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, MINUS, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "MINUS instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            data[data_ptr]--;
            return state<ins_ptr + 1, op_at<ins_ptr + 1, op, ops...>::operation, op, ops...>
                    ::exec(data, data_ptr, output, input);
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, DATA_RIGHT, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "DATA_RIGHT instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            return state<ins_ptr + 1, op_at<ins_ptr + 1, op, ops...>::operation, op, ops...>
                    ::exec(data, data_ptr + 1, output, input);
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, DATA_LEFT, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "DATA_LEFT instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            return state<ins_ptr + 1, op_at<ins_ptr + 1, op, ops...>::operation, op, ops...>
                    ::exec(data, data_ptr - 1, output, input);
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, IN, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "IN instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            data[data_ptr] = getc(input);
            return state<ins_ptr + 1, op_at<ins_ptr + 1, op, ops...>::operation, op, ops...>
                    ::exec(data, data_ptr, output, input);
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, OUT, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "OUT instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            fputc(data[data_ptr], output);
            return state<ins_ptr + 1, op_at<ins_ptr + 1, op, ops...>::operation, op, ops...>
                    ::exec(data, data_ptr, output, input);
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, LOOP_OPEN, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "LOOP_OPEN instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            constexpr bfop next_op = op_at<ins_ptr + 1, op, ops...>::operation;

            if (data[data_ptr] == 0) {
                constexpr unsigned int next_ptr = scan_forward<ins_ptr + 1, 1, next_op, op, ops...>::new_ptr;
                return state<next_ptr, op_at<next_ptr, op, ops...>::operation, op, ops...>
                        ::exec(data, data_ptr, output, input);
            } else {
                return state<ins_ptr + 1, next_op, op, ops...>
                        ::exec(data, data_ptr, output, input);
            }
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    template <unsigned int ins_ptr, bfop op, bfop... ops>
    struct state<ins_ptr, LOOP_CLOSE, op, ops...> {
        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr, FILE* output, FILE* input) {
            #ifdef LIBBRAIN_BUILD_WITH_DEBUG

            fprintf(stderr, "LOOP_CLOSE instruction reached at %i, data_ptr = %i\n", ins_ptr, data_ptr);

            #endif

            constexpr bfop prev_op = op_at<ins_ptr - 1, op, ops...>::operation;

            if (data[data_ptr] == 0) {
                return state<ins_ptr + 1, op_at<ins_ptr + 1, op, ops...>::operation, op, ops...>
                        ::exec(data, data_ptr, output, input);
            } else {
                constexpr unsigned int next_ptr = scan_backward<ins_ptr - 1, 1, prev_op, op, ops...>::new_ptr + 2;
                return state<next_ptr, op_at<next_ptr, op, ops...>::operation, op, ops...>
                        ::exec(data, data_ptr, output, input);
            }
        }

        static constexpr inline int exec(unsigned char data[], unsigned int data_ptr) {
            return exec(data, data_ptr, stdout, stdin);
        }
    };

    constexpr bfop from_char(char c) {
        switch (c) {
            case '+':
                return bfop::PLUS;
            case '-':
                return bfop::MINUS;
            case '>':
                return bfop::DATA_RIGHT;
            case '<':
                return bfop::DATA_LEFT;
            case '.':
                return bfop::OUT;
            case ',':
                return bfop::IN;
            case '[':
                return bfop::LOOP_OPEN;
            case ']':
                return bfop::LOOP_CLOSE;
            default:
                return bfop::NOP;
        }
    }
}

template <typename T, T... chars>
libbrain::state<0, libbrain::bfop::NOP, libbrain::bfop::NOP, libbrain::from_char(chars)..., libbrain::bfop::END>
constexpr operator""_libbrain_state_literal() { return {}; }

#define BRAIN_COMPILE(...) #__VA_ARGS__ ""_libbrain_state_literal

#define BRAIN_PROGRAM libbrain::state

#define BRAIN_ALLOC_TAPE(amount) new unsigned char[amount]()

#ifdef LIBBRAIN_BUILD_WITH_DEBUG
#undef LIBBRAIN_BUILD_WITH_DEBUG
#endif

#endif