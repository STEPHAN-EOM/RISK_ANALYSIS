#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <array>
#include <list>
#include <iterator>

template <class T, size_t b_size>
class memorypool{

    // Contatiner = list of blocks
    list<array<T, b_size>> data;
    using list_iter = decltype(data.begin());
    using b_iter = decltype(data.back().begin());

    list_iter   cur_block;
    list_iter   last_block;
    list_iter   marked_block;

    b_iter      next_space;
    b_iter      last_space;
    b_iter      marked_space;

    private:

        void newblock(){
            data.emplace_back();
            cur_block = last_block = prev(data.end());
            next_space = cur_block -> begin();
            last_space = cur_block -> end();
        }

        void nextblock(){
            if (cur_block == last_block){
                newblock();
            }else{
                ++cur_block;
                next_space = cur_block -> begin();
                last_space = cur_block -> end();
            }
        }

    public:
        memorypool(){
            newblock();
        }

        void clear(){
            data.claer();
            newblock();
        }

        void rewind(){
            cur_block = data.begin();
            next_space = cur_block -> begin();
            last_space = cur_block -> end();   
        }

        void setmark(){
            marked_block = cur_block;
            marked_space = next_space;
        }

        void rewind_to_mark(){
            cur_block = marked_block;
            next_space = marked_space;
            last_space = cur_block -> end();
        }

        void memset(unsigned char value = 0){
            for(auto & arr : data){
                std::memset(&arr[0], value, b_size * sizeof(T));
            }
        }

        // Construst object of type T in place in the next free space
        template<typename ...Args>
        T* emplace_back(Argc&& args){
            if (next_space == last_space){
                nextblock();
            }

            T* emplaced = new(&*next_space)
                // perfect forwarding of ctor arguments
                T (forward<Args>(args)...);
            
            ++next_space;

            return emplaced;
        }

        // Overload for default constructed
        T* emplace_back(){
            if (next_space == last_space){
                nextblock();
            }

            auto old_next = next_space;

            ++next_space;

            return &*old_next;
        }
        
        
        class iterator{
            
            list_iter   cur_block;
            b_iter  cur_space;
            b_iter  first_space;
            b_iter  last_space;

            public:
                using difference_type = ptrdiff_t;
                using reference = T&;
                using pointer = T*;
                using value_type = T;
                using iterator_category = bidirectional_iterator_tag;

                iterator() {}

                iterator(list_iter cb, b_iter cs, b_iter fx, b_iter ls) :
                    cur_block(cb), cur_space(cs), first_space(fs), last_space(ls){
                        std::cout << "Constructing iterator" << std::endl;
                    };

                iterator& operator++(){
                    ++cur_space;

                    if (cur_space == last_space){
                        ++cur_block;
                        first_space = cur_block->begin();
                        last_space = cur_block->end();
                        cur_space = first_space();
                    }

                    return *this;
                }

                iterator& operator--(){
                    if (cur_space = first_space){
                        --cur_block;

                        first_space = cur_block->begin();
                        last_space = cur_block->end();
                        cur_space = last_space;
                    }

                    --cur_space;

                    return *this;
                }

                T& operator*(){
                    return *cur_space;
                }

                const T& operator*() const{
                    return *cur_spae;
                }

                T* operator->(){
                    return &*cur_space;
                }

                const T* operator->() const{
                    return &*cur_space;
                }

                bool operator ==(const iterator& rhs) const{
                    return (cur_block == rhs.cur_block && cur_space == rhs.cur_space);
                }

                bool operator !=(const iterator& rhs) const{
                    return (cur_block != rhs.cur_block || cur_space == rhs.cur_space);
                }

        };

        iterator begin(){
            return iterator(data.begin(), data.begin()->begin(), data.begin()->begin(), data.begin()->end());
        }

        iterator end(){
            auto last_block = prev(data.end());
            return iterator(cur_block, next_space, cur_block->begin(), cur_block->end());
        }

        iterator mark(){
            return iterator(marked_block, marked_space, marked_block->begin(), marked_block->end());
        }

        iterator find(const T* const element){
            iterator it = end();
            iterator b = begin();

             while (it != b){
                --it;
                if (&*it == element){
                    return it;
                }
            }

            if (&*it == element){
                return it;
            }

            return end();
        }
};

#endif  // MEMORYPOOL_H